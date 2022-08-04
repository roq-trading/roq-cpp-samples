/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/vwap/flags/flags.hpp"

#include <absl/flags/flag.h>

ABSL_FLAG(  //
    double,
    quantity,
    1.0,
    "vwap quantity");

namespace roq {
namespace samples {
namespace vwap {
namespace flags {

double Flags::quantity() {
  static double const result = absl::GetFlag(FLAGS_quantity);
  return result;
}

}  // namespace flags
}  // namespace vwap
}  // namespace samples
}  // namespace roq
