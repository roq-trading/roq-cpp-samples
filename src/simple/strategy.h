/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <string>
#include <tuple>

#include "simple/base.h"
#include "simple/config.h"

namespace examples {
namespace simple {

class Strategy final : public BaseStrategy {
 public:
  Strategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config);

 protected:
  void update(const MarketData& market_data) override;

 private:
  double compute(const MarketData& market_data) const;

 private:
  typedef std::tuple<
      roq::TradeDirection,
      double,
      double,
      std::string> create_order_args_t;
  create_order_args_t create_order_args(
      int sign_signal,
      const roq::Layer& best) const;

 private:
  static void write_signal(
      const MarketData& market_data,
      double value,
      double signal);
  static void write_create_order(
      const MarketData& market_data,
      const create_order_args_t& args);

 private:
  const bool _weighted;
  const double _threshold;
  const double _quantity;
  double _previous = std::numeric_limits<double>::quiet_NaN();
};

}  // namespace simple
}  // namespace examples
