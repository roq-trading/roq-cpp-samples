/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/state.hpp"

#include "roq/algo/strategies/base.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Spread final : public Base {
 public:
  Spread(framework::Dispatcher &, const framework::State &, const std::string_view &routing_id, const CreateOrder &);

  void operator()(const ModifyOrder &);
  void operator()(const CancelOrder &);

 protected:
  void operator()(const Ready &) override;
  void operator()(const NotReady &) override;

  void operator()(const Event<Timer> &) override;

  void operator()(const Event<TopOfBook> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;

  void operator()(const Event<PositionUpdate> &) override;

  void update();

 protected:
  double current_spread() const;

 private:
  const Side side_;
  const double quantity_;
  const double price_;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
