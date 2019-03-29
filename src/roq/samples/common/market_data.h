/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace samples {
namespace common {

struct MarketData final {
  size_t index;
  const char *exchange;
  const char *symbol;
  double tick_size;
  double multiplier;
  Layer depth[5];
  double price;
  double volume;
  double turnover;
  Side side;
  std::chrono::nanoseconds exchange_time;
};

}  // namespace common
}  // namespace samples
}  // namespace roq
