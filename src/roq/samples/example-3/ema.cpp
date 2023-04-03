/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-3/ema.hpp"

#include <algorithm>
#include <cmath>

#include <roq/numbers.hpp>

#include "roq/samples/example-3/flags/flags.hpp"

namespace roq {
namespace samples {
namespace example_3 {

// === IMPLEMENTATION ===

EMA::EMA(double alpha) : alpha_{alpha}, countdown_{flags::Flags::warmup()} {
}

void EMA::reset() {
  value_ = NaN;
  countdown_ = flags::Flags::warmup();
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
