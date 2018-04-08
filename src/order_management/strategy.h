/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>

#include "order_management/base.h"

namespace examples {
namespace order_management {

class Strategy final : public BaseStrategy {
 public:
  using BaseStrategy::BaseStrategy;

 protected:
  void reset() override;
  void update(const MarketData&) override;

 private:
  void try_trade(
    roq::TradeDirection direction,
    double quantity,
    double price);

 private:
  const double _quantity = 1.0;
  double _threshold = 2.0;  // multiplier on tick-size
  double _previous = std::numeric_limits<double>::quiet_NaN();
};

}  // namespace order_management
}  // namespace examples
