/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/ema.h"

#include <algorithm>
#include <cmath>

#include "roq/samples/example-3/flags.h"

namespace roq {
namespace samples {
namespace example_3 {

EMA::EMA(double alpha) : alpha_(alpha), countdown_(Flags::warmup()) {
}

void EMA::reset() {
  value_ = NaN;
  countdown_ = Flags::warmup();
}

double EMA::update(double value) {
  countdown_ = std::max<uint32_t>(1, countdown_) - 1;
  if (std::isnan(value_))
    value_ = value;  // initialize
  else
    value_ = alpha_ * value + (1.0 - alpha_) * value_;
  return value_;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
