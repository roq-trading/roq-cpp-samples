/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-4/config.hpp"

namespace roq {
namespace samples {
namespace example_4 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = settings_.symbols,
      .exchange = settings_.exchange,
  });
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
