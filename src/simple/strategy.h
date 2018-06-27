/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <string>
#include <tuple>

#include "simple/config.h"

#include "common/base_strategy.h"

namespace examples {
namespace simple {

class Strategy final : public common::BaseStrategy {
 public:
  Strategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& gateway,
      const Config& config);

 protected:
  void update(const std::chrono::system_clock::time_point now) override;
  void update(const common::MarketData& market_data) override;

 private:
  double compute(const roq::Layer *depth, size_t length) const;

 private:
  static void write_signal(
      roq::time_point_t exchange_time,
      const roq::Layer& best,
      double value,
      double signal);
  static void write_order(
      roq::time_point_t exchange_time,
      roq::Side side,
      double quantity,
      double price);

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
