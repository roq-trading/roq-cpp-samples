/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/samples/experiment/settings.hpp"

#include "roq/samples/experiment/test.hpp"

namespace roq {
namespace samples {
namespace experiment {

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
