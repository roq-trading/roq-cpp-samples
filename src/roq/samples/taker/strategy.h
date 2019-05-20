/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <atomic>
#include <string>

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

  double compute_mid_price(const Layer *depth, size_t length) const;

  void write(Metrics& metrics) const final;

 private:
  const double _quantity = 1.0;  // FIXME(thraneh): from config
  const bool _weighted = false;
  const double _threshold = 1.0e-8;
  double _previous = 0.0;  // keep track of previous mid price
  // Metrics:
  std::atomic<double> _signal {};
};

}  // namespace taker
}  // namespace samples
}  // namespace roq
