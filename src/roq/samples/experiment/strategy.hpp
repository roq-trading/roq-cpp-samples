/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/samples/experiment/settings.hpp"

#include "roq/samples/experiment/test.hpp"

namespace roq {
namespace samples {
namespace experiment {

// initially:
//   place a bid at a price expected to be the low of the period
//   place an offer at a price expected to be the high of the period
//
// if one order is completed:
//   the price of the other order will be modified to a 1/3 retracement of the initial range
//
// if both orders are completed:
//   done
//
// if either of the orders have failed:
//   fail
//
// if too many retries:
//   fail
//
// if any request fails:
//   delay and retry if due to rate-limiter
//   fail after N unsuccessful retries

struct Strategy final : public Handler {
  Strategy(Dispatcher &, Settings const &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<Ready> const &) override;

  void operator()(Event<ReferenceData> const &, Market const &) override;
  void operator()(Event<MarketStatus> const &, Market const &) override;
  void operator()(Event<TopOfBook> const &, Market const &) override;

  void operator()(Event<OrderUpdate> const &, Order const &) override;

 protected:
  void start();
  void stop();
  // -
  void maybe_create_orders(Market const &);

 private:
  Dispatcher &dispatcher_;
  Settings const &settings_;
  bool ready_ = {};

  struct Leg final {
    Leg(Strategy &, Side, double quantity);

    bool ready() const;
    bool done() const;

    void operator()(Market const &);

    void create(double price);
    void create_helper(size_t retry_counter);

    void modify(double price);
    void modify_helper(size_t retry_counter);

    void update(Event<OrderUpdate> const &, Order const &);

   private:
    Strategy &strategy_;
    Side const side_;
    double const quantity_;
    double price_ = NaN;
    enum class State {
      UNDEFINED,
      READY,
      CREATING,
      MODIFYING,
      WORKING,
      DONE,
      FAILED,
    } state_ = {};
    std::unique_ptr<Order> order_;
  };

  friend Leg;  // XXX TODO move to Shared

  Leg leg_1_;
  Leg leg_2_;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
