/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/controller.hpp"

#include <fmt/format.h>

#include <utility>

#include "roq/logging.hpp"

#include "roq/samples/io-context/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

Controller::Controller(client::Dispatcher &dispatcher, io::Context &context)
    : dispatcher_(dispatcher), context_(context)
// XXX TODO sender_(context_.create_udp_sender())
{
}

void Controller::operator()(Event<Timer> const &) {
  context_.drain();
  // garbage collection
  for (auto session_id : shared_.sessions_to_remove)
    sessions_.erase(session_id);
  shared_.sessions_to_remove.clear();
}

void Controller::operator()(Event<TopOfBook> const &event) {
  if (!sender_)
    return;
  auto &[message_info, top_of_book] = event;
  auto &layer = top_of_book.layer;
  // json message
  // note! you can optimize this by pre-allocating a buffer and use fmt::format_to
  auto message = fmt::format(
      R"(["{}",{},{},{},{}])"sv,
      top_of_book.symbol,
      layer.bid_price,
      layer.bid_quantity,
      layer.ask_price,
      layer.ask_quantity);
  // note! you should use a higher verbosity level here to make it possible to avoid some logging
  log::info<0>("{}"sv, message);
  // broadcast
  (*sender_).send({reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)});
}

void Controller::operator()(io::net::tcp::Connection::Factory &factory) {
  auto session_id = ++next_session_id_;
  auto session = std::make_unique<Session>(session_id, factory, shared_);
  sessions_.try_emplace(session_id, std::move(session));
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
