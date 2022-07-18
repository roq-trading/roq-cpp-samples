/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/controller.hpp"

#include <fmt/format.h>

#include <utility>

#include "roq/logging.hpp"

#include "roq/json/top_of_book.hpp"

#include "roq/samples/io-context/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

Controller::Controller(client::Dispatcher &dispatcher, io::Context &context)
    : dispatcher_(dispatcher), context_(context),
      listener_(context_.create_tcp_listener(*this, io::NetworkAddress{flags::Flags::ws_port()})),
      sender_(context_.create_udp_sender(*this, io::NetworkAddress{flags::Flags::udp_port()})) {
}

void Controller::operator()(Event<Timer> const &event) {
  // drain enqueued events
  context_.drain();
  // garbage collect disconnected sessions
  if (next_garbage_collection_ < event.value.now) {
    next_garbage_collection_ = event.value.now + 1s;
    for (auto session_id : shared_.sessions_to_remove) {
      log::info("Removing session_id={}..."sv, session_id);
      sessions_.erase(session_id);
    }
    shared_.sessions_to_remove.clear();
  }
}

void Controller::operator()(Event<TopOfBook> const &event) {
  // send json encoded update + line feed
  send("{}\n"sv, json::TopOfBook{event});
}

template <typename... Args>
void Controller::send(fmt::format_string<Args...> const &fmt, Args &&...args) {
  buffer_.clear();
  fmt::format_to(std::back_inserter(buffer_), fmt, std::forward<Args>(args)...);
  std::string_view message{std::data(buffer_), std::size(buffer_)};
  log::info<3>("{}"sv, message);
  (*sender_).send({reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)});
}

void Controller::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

void Controller::operator()(io::net::tcp::Connection::Factory &factory) {
  auto session_id = ++next_session_id_;
  log::info("Adding session_id={}..."sv, session_id);
  auto session = std::make_unique<Session>(session_id, factory, shared_);
  sessions_.try_emplace(session_id, std::move(session));
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
