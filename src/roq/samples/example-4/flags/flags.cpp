/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-4/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange name");

ABSL_FLAG(  //
    std::string,
    symbols,
    ".*",
    "regex used to subscribe symbols");

namespace roq {
namespace samples {
namespace example_4 {
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
}  // namespace example_4
}  // namespace samples
}  // namespace roq
