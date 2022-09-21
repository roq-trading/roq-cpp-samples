/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/controller.hpp"

#include <utility>

#include "roq/logging.hpp"

#include "roq/json/top_of_book.hpp"

#include "roq/samples/io-context/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

namespace {
auto create_sender(auto &handler, auto &context) {
  auto network_address = io::NetworkAddress{flags::Flags::udp_port()};
  auto socket_options = Mask{
      io::SocketOption::REUSE_ADDRESS,
  };
  return context.create_udp_sender(handler, network_address, socket_options);
}
auto create_listener(auto &handler, auto &context) {
  auto network_address = io::NetworkAddress{flags::Flags::ws_port()};
  return context.create_tcp_listener(handler, network_address);
}
}  // namespace

Controller::Controller(client::Dispatcher &dispatcher, io::Context &context)
    : dispatcher_(dispatcher), context_(context), sender_(create_sender(*this, context)),
      listener_(create_listener(*this, context_)) {
}

// client::Handler

void Controller::operator()(Event<Timer> const &event) {
  context_.drain();
  remove_zombies(event.value.now);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  if (flags::Flags::filter_symbols() && shared_.symbols.find(event.value.symbol) == std::end(shared_.symbols))
    return;
  send("{}\n"sv, json::TopOfBook{event});
}

// io::net::udp::Sender::Handler

void Controller::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

// io::net::tcp::Listener::Handler

void Controller::operator()(io::net::tcp::Connection::Factory &factory) {
  auto session_id = ++next_session_id_;
  log::info("Adding session_id={}..."sv, session_id);
  auto session = std::make_unique<Session>(session_id, factory, shared_);
  sessions_.try_emplace(session_id, std::move(session));
}

// utilities

void Controller::remove_zombies(std::chrono::nanoseconds now) {
  if (now < next_garbage_collection_)
    return;
  next_garbage_collection_ = now + 1s;
  for (auto session_id : shared_.sessions_to_remove) {
    log::info("Removing session_id={}..."sv, session_id);
    sessions_.erase(session_id);
  }
  shared_.sessions_to_remove.clear();
}

template <typename... Args>
void Controller::send(fmt::format_string<Args...> const &fmt, Args &&...args) {
  buffer_.clear();
  fmt::format_to(std::back_inserter(buffer_), fmt, std::forward<Args>(args)...);
  std::string_view message{std::data(buffer_), std::size(buffer_)};
  log::info<3>("{}"sv, message);
  (*sender_).send({reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)});
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
