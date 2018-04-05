/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include "order_management/base.h"

namespace examples {
namespace order_management {

class Strategy final : public BaseStrategy {
 public:
  using BaseStrategy::BaseStrategy;

 protected:
  void update_signal(const roq::MarketByPrice&) override;
  void try_trade() override;

 private:
  roq::Layer _best = {};
};

}  // namespace order_management
}  // namespace examples
