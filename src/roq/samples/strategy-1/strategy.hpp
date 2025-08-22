/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/strategy/bridge.hpp"

#include "roq/samples/strategy-1/settings.hpp"

#include "roq/samples/strategy-1/shared.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

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

struct Strategy final : public strategy::Bridge::Handler {
  explicit Strategy(Shared &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  // strategy::Bridge::Handler
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<Ready> const &) override;
  void operator()(Event<ReferenceData> const &, strategy::Market const &) override;
  void operator()(Event<MarketStatus> const &, strategy::Market const &) override;
  void operator()(Event<TopOfBook> const &, strategy::Market const &) override;
  void operator()(Event<OrderUpdate> const &, strategy::Order const &) override;

 protected:
  void start();
  void stop();
  // -
  void maybe_create_orders(strategy::Market const &);

 private:
  Shared &shared_;

  bool ready_ = {};

  struct Leg final {
    Leg(Shared &, Side, double quantity);

    bool ready() const;
    bool finished() const;

    void operator()(strategy::Market const &);

    void submit_order(double price);
    void submit_order_helper(size_t retry_counter);

    void modify_order(double price);
    void modify_order_helper(size_t retry_counter);

    void update(Event<OrderUpdate> const &, strategy::Order const &);

   private:
    Shared &shared_;
    Side const side_;
    double const quantity_;
    double price_ = NaN;  // note! dynamic
    enum class State {
      UNDEFINED,
      READY,     // ready for action
      CREATING,  // request was sent to create an order on the exchange
      CHANGING,  // request was sent to change order attributes on the exchange
      WAITING,   // waiting for order to be filled
      FINISHED,  // finished successfully
      FAILED,    // something failed
    } state_ = {};
    std::unique_ptr<strategy::Order> order_;
  };

  friend Leg;  // XXX TODO move to Shared

  Leg leg_1_;
  Leg leg_2_;
};

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
