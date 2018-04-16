/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <string>
#include <tuple>

#include "simple/config.h"

#include "common/simple_strategy.h"

namespace examples {
namespace simple {

class Strategy final : public common::SimpleStrategy {
 public:
  Strategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& gateway,
      const Config& config);

 protected:
  void update(const common::MarketData& market_data) override;

 private:
  double compute(const roq::Layer *depth, size_t length) const;

 private:
  typedef std::tuple<
      roq::Side,
      double,
      double,
      std::string> create_order_args_t;
  create_order_args_t create_order_args(
      int sign_signal,
      const roq::Layer& best) const;

 private:
  static void write_signal(
      roq::time_point_t exchange_time,
      const roq::Layer& best,
      double value,
      double signal);
  static void write_create_order(
      roq::time_point_t exchange_time,
      const create_order_args_t& args);

 private:
  // Configuration.
  const bool _weighted;
  const double _threshold;
  const double _quantity;
  // State management.
  double _previous = std::numeric_limits<double>::quiet_NaN();
};

}  // namespace simple
}  // namespace examples
