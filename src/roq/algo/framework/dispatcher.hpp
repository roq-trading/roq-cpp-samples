/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace framework {

class Dispatcher {
 public:
  virtual uint32_t next_order_id() const = 0;

  virtual void operator()(const CreateOrder &) = 0;
  virtual void operator()(const ModifyOrder &) = 0;
  virtual void operator()(const CancelOrder &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
