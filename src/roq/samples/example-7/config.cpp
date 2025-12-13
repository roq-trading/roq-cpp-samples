/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-7/config.hpp"

namespace roq {
namespace samples {
namespace example_7 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  handler(
      client::Symbol{
          .regex = settings_.symbol,
          .exchange = settings_.exchange,
      });
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
