/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange");

ABSL_FLAG(  //
    std::string,
    symbol,
    ".*",
    "symbol");

ABSL_FLAG(  //
    uint16_t,
    udp_port,
    1234,
    "port");

ABSL_FLAG(  //
    uint16_t,
    ws_port,
    2345,
    "port");

namespace roq {
namespace samples {
namespace io_context {
namespace flags {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

uint16_t Flags::ws_port() {
  static const uint16_t result = absl::GetFlag(FLAGS_ws_port);
  return result;
}

uint16_t Flags::udp_port() {
  static const uint16_t result = absl::GetFlag(FLAGS_udp_port);
  return result;
}

}  // namespace flags
}  // namespace io_context
}  // namespace samples
}  // namespace roq
