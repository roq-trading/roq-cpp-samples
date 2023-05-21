/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/python/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange");

ABSL_FLAG(  //
    std::string,
    symbols,
    "BTC-PERPETUAL",
    "regex used to subscribe symbols");

namespace roq {
namespace samples {
namespace python {
namespace flags {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbols() {
  static const std::string result = absl::GetFlag(FLAGS_symbols);
  return result;
}

}  // namespace flags
}  // namespace python
}  // namespace samples
}  // namespace roq
