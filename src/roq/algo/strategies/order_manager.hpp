/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Base;

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
  double traded_quantity_ = 0.0;
  //
  double target_quantity_ = NaN;
  double target_price_ = NaN;
  //
  uint32_t order_id_ = {};
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
