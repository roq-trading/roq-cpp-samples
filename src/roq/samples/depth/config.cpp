/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/depth/config.hpp"

namespace roq {
namespace samples {
namespace depth {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.symbol,
      .exchange = settings_.exchange,
  });
}

}  // namespace depth
}  // namespace samples
}  // namespace roq
