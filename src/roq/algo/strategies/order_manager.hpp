/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Base;

struct OrderManager final {
  OrderManager(Base &, size_t index, const Side &);

  void set_target(double quantity, double price);

  void start();
  void stop();

  void operator()(const Event<OrderAck> &);
  void operator()(const Event<OrderUpdate> &);

  void operator()(const Event<PositionUpdate> &);

 private:
  Base &base_;
  const size_t index_;
  const Side side_;
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
