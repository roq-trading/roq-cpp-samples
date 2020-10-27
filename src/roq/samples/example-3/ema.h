/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <cstdint>
#include <limits>

#include "roq/samples/example-3/options.h"

namespace roq {
namespace samples {
namespace example_3 {

class EMA final {
 public:
  explicit EMA(double alpha);

  EMA(const EMA &) = delete;
  EMA(EMA &&) = default;

  operator double() const { return value_; }

  void reset();

  bool is_ready() const { return countdown_ == 0; }

  double update(double value);

 private:
  const double alpha_;
  double value_ = std::numeric_limits<double>::quiet_NaN();
  uint32_t countdown_ = FLAGS_warmup;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
