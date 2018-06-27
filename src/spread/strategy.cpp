/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "spread/strategy.h"

namespace examples {
namespace spread {

Strategy::Strategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config)
    : common::BaseStrategy(dispatcher, gateway, config.config) {
}

void Strategy::update(std::chrono::system_clock::time_point now) {
  // the timer callback is useful for e.g. unwinding positions
}

void Strategy::update(const common::MarketData& market_data) {
}

}  // namespace spread
}  // namespace examples
