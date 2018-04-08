/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <string>

#include "simple/base.h"
#include "simple/config.h"

namespace examples {
namespace simple {

class Strategy final : public BaseStrategy {
 public:
  Strategy(
    roq::Strategy::Dispatcher& dispatcher,
    const Config& config,
    const std::string& gateway);

 protected:
  void reset() override;
  void update(const MarketData&) override;

 private:
  void try_trade(
    roq::TradeDirection direction,
    double quantity,
    double price);

 private:
  const double _threshold;
  const double _quantity;
  double _previous = std::numeric_limits<double>::quiet_NaN();
};

}  // namespace simple
}  // namespace examples
