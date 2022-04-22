/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/zeromq/strategy.hpp"

#include <zmq.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fmt/format.h>

#include "roq/samples/zeromq/flags/flags.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace zeromq {

namespace {
auto create_socket(auto &context) {
  zmq::Socket result(context, ZMQ_PUB);
  result.bind(flags::Flags::endpoint());
  return result;
}
}  // namespace

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher), socket_(create_socket(context_)) {
}

void Strategy::operator()(const Event<TopOfBook> &event) {
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
  // send
  socket_.send(std::data(message), std::size(message), ZMQ_DONTWAIT);
}

}  // namespace zeromq
}  // namespace samples
}  // namespace roq
