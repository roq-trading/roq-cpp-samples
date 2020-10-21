/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/ema.h"

#include <algorithm>
#include <cmath>

#include "roq/samples/example-3/options.h"

namespace roq {
namespace samples {
namespace example_3 {

EMA::EMA(double alpha) : _alpha(alpha) {
}

void EMA::reset() {
  _value = std::numeric_limits<double>::quiet_NaN();
  _countdown = FLAGS_warmup;
}

double EMA::update(double value) {
  _countdown = std::max<uint32_t>(1, _countdown) - uint32_t{1};
  if (std::isnan(_value))
    _value = value;
  else
    _value = _alpha * value + (1.0 - _alpha) * _value;
  return _value;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
