/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <vector>

#include "roq/samples/common/strategy.h"

namespace roq {
namespace samples {
namespace taker {

// A higher strategy abstraction based on the the implementation
// in the common library

class Strategy final : public common::Strategy {
 public:
  Strategy(
      client::Dispatcher& dispatcher,
      const common::Config& config);

 protected:
  void update(std::chrono::nanoseconds now) final;
  void update(const CreateOrderAck&) final;
  void update(const OrderUpdate&) final;
  void update(const TradeUpdate&) final;
  void update(const common::MarketData&) final;

  double compute(const Layer *depth, size_t length) const;

  void write_signal(
      std::chrono::nanoseconds exchange_time,
      const Layer& best,
      double value,
      double signal);
  void write_order(
      std::chrono::nanoseconds exchange_time,
      Side side,
      double quantity,
      double price);

 private:
  const double _quantity = 1.0;  // FIXME(thraneh): from config
  const bool _weighted = false;
  const double _threshold = 1.0e-8;
  double _previous = 0.0;
};

}  // namespace taker
}  // namespace samples
}  // namespace roq
