/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-3/ema.hpp"

#include <algorithm>
#include <cmath>

namespace roq {
namespace samples {
namespace example_3 {

// === IMPLEMENTATION ===

EMA::EMA(double alpha, uint32_t warmup) : alpha_{alpha}, warmup_{warmup}, countdown_{warmup_} {
}

void EMA::reset() {
  value_ = NaN;
  countdown_ = warmup_;
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
