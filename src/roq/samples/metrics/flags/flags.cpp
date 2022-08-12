/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/metrics/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    symbols,
    "^BTC-.*$",
    "regex used to subscribe symbols");

namespace roq {
namespace samples {
namespace metrics {
namespace flags {

std::string_view Flags::symbols() {
  static const std::string result = absl::GetFlag(FLAGS_symbols);
  return result;
}

}  // namespace flags
}  // namespace metrics
}  // namespace samples
}  // namespace roq
