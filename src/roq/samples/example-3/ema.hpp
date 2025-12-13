/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <cstdint>

#include "roq/limits.hpp"

namespace roq {
namespace samples {
namespace example_3 {

struct EMA final {
  EMA(double alpha, uint32_t warmup);

  EMA(EMA &&) = default;
  EMA(const EMA &) = delete;

  operator double() const { return value_; }

  void reset();

  bool is_ready() const { return countdown_ == 0; }

  double update(double value);

 private:
  double const alpha_;
  uint32_t const warmup_;

  double value_ = NaN;
  uint32_t countdown_;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
