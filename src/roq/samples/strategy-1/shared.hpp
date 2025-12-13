/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/strategy/tools/rate_limiter.hpp"

#include "roq/samples/strategy-1/settings.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

struct Controller;

struct Shared final {
  Shared(Settings const &, Controller &);

  Settings const &settings;
  Controller &controller;

  strategy::tools::RateLimiter rate_limiter;
};

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
