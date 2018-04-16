/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace common {

struct MarketData final {
  const size_t index;
  const char *exchange;
  const char *symbol;
  roq::Layer depth[roq::MAX_DEPTH];
  double price;
  double volume;
  double turnover;
  roq::TradeDirection direction;
  roq::time_point_t exchange_time;
  uint16_t channel;
};

}  // namespace common
}  // namespace examples
