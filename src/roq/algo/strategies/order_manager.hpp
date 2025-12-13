/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace strategies {

struct Base;

struct OrderManager final {
  OrderManager(Base &, size_t index, Side const &);

  void set_target(double quantity, double price);

  void start();
  void stop();

  void operator()(Event<OrderAck> const &);
  void operator()(Event<OrderUpdate> const &);

  void operator()(Event<PositionUpdate> const &);

 private:
  Base &base_;
  size_t const index_;
  Side const side_;
  //
  double target_quantity_ = NaN;
  double target_price_ = NaN;
  //
  uint64_t order_id_ = {};
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
