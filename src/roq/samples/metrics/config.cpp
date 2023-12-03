/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/metrics/config.hpp"

namespace roq {
namespace samples {
namespace metrics {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.symbols,
      .exchange = {},
  });
}

}  // namespace metrics
}  // namespace samples
}  // namespace roq
