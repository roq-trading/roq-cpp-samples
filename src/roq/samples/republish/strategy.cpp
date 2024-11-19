/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/republish/strategy.hpp"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fmt/format.h>

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace republish {

// === HELPERS ===

// convenience wrappers around some libc socket functions
// note! not very generic -- just for the example
namespace {
auto create_socket(auto &settings, auto &address) {
  // create
  auto fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (fd < 0)
    log::system_error("Failed to create socket"sv);
  // bind
  address.sin_family = AF_INET;
  address.sin_port = htons(settings.port);
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  if (::bind(fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
    log::system_error("Failed to bind socket"sv);
  // non-blocking
  auto flags = ::fcntl(fd, F_GETFL, 0);
  if (flags < 0)
    log::system_error("Failed to get flags"sv);
  flags |= O_NONBLOCK;
  if (::fcntl(fd, F_SETFL, flags) < 0)
    log::system_error("Failed to set flags"sv);
  return fd;
}

void close_socket(auto fd) {
  if (fd > 0)
    ::close(fd);
}

void socket_sendto(auto const socket, auto const &address, auto const &message) {
  auto res = ::sendto(socket, std::data(message), std::size(message), 0, reinterpret_cast<struct sockaddr const *>(&address), sizeof(address));
  if (res < 0)
    log::system_error("Failed to send"sv);
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher, Settings const &settings) : dispatcher_{dispatcher}, socket_{create_socket(settings, address_)} {
}

Strategy::~Strategy() {
  // note! best effort
  try {
    close_socket(socket_);
  } catch (...) {
  }
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  auto &layer = top_of_book.layer;
  // json message
  // note! you can optimize this by pre-allocating a buffer and use fmt::format_to
  auto message = fmt::format(R"(["{}",{},{},{},{}])"sv, top_of_book.symbol, layer.bid_price, layer.bid_quantity, layer.ask_price, layer.ask_quantity);
  // note! you should use a higher verbosity level here to make it possible to avoid some logging
  log::info<0>("{}"sv, message);
  // broadcast
  socket_sendto(socket_, address_, message);
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
