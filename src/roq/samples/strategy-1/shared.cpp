/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/strategy-1/shared.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace strategy_1 {

// === HELPERS ===

namespace {
auto create_rate_limiter(auto &settings) {
  auto config = strategy::tools::RateLimiter::Config{
      .request_limit = settings.rate_limiter.request_limit,
      .monitor_period = settings.rate_limiter.monitor_period,
      .ban_period = settings.rate_limiter.ban_period,
  };
  return strategy::tools::RateLimiter{config};
}
}  // namespace

// === IMPLEMENTATION ===

Shared::Shared(Settings const &settings, Controller &controller) : settings{settings}, controller{controller}, rate_limiter{create_rate_limiter(settings)} {
}

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
