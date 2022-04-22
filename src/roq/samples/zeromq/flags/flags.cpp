/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/zeromq/flags/flags.hpp"

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
    std::string,
    endpoint,
    "tcp://localhost:1234",
    "endpoint (zeromq)");

namespace roq {
namespace samples {
namespace zeromq {
namespace flags {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

std::string_view Flags::endpoint() {
  static const std::string result = absl::GetFlag(FLAGS_endpoint);
  return result;
}

}  // namespace flags
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
