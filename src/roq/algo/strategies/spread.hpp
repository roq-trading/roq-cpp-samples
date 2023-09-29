/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/state.hpp"

#include "roq/algo/strategies/base.hpp"

namespace roq {
namespace algo {
namespace strategies {

struct Spread final : public Base {
  Spread(framework::Dispatcher &, framework::State const &, std::string_view const &routing_id, CreateOrder const &);

  void operator()(ModifyOrder const &);
  void operator()(CancelOrder const &);

 protected:
  void operator()(Ready const &) override;
  void operator()(NotReady const &) override;

  void operator()(Event<Timer> const &) override;

  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;

  void operator()(Event<PositionUpdate> const &) override;

  void update();

 protected:
  double current_spread() const;

 private:
  Side const side_;
  double const quantity_;
  double const price_;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
