/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <limits>
#include <string>
#include <tuple>

#include "spread/config.h"

#include "common/base_strategy.h"

namespace examples {
namespace spread {

class Strategy final : public common::BaseStrategy {
 public:
  Strategy(
      roq::Client::Dispatcher& dispatcher,
      const std::string& gateway,
      const Config& config);

 protected:
  void update(std::chrono::system_clock::time_point now) override;
  void update(const common::MarketData& market_data) override;
};

}  // namespace spread
}  // namespace examples
