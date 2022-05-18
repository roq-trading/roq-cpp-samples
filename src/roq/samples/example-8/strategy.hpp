/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_8 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Timer> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<TradeSummary> const &) override;

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
