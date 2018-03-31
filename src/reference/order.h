/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace reference {

class Order {
 public:
  Order(roq::TradeDirection direction, double quantity)
      : direction(direction), remaining_quantity(quantity) {}
  const roq::TradeDirection direction;
  enum { Requested, Sent, Accepted, Deleting } state = Requested;
  double remaining_quantity;
  uint32_t gateway_order_id = 0;
  double fill(const roq::OrderUpdate& order_update);
 private:
  double _traded_quantity = 0.0;
};

}  // namespace reference
}  // namespace examples

