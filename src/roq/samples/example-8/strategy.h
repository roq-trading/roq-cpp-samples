/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_8 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<Timer> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<TradeSummary> &) override;

 private:
  client::Dispatcher &dispatcher_;
  std::unique_ptr<cache::MarketByPrice> market_by_price_;
  bool active_ = false;
  double limit_price_ = NaN;
  double quantity_when_placed_ = NaN;
  double traded_since_placed_ = 0.0;
  // best bid/ask
  double best_bid_ = NaN;
  double best_ask_ = NaN;
  // timer
  std::chrono::nanoseconds next_info_ = {};
};

}  // namespace example_8
}  // namespace samples
}  // namespace roq
