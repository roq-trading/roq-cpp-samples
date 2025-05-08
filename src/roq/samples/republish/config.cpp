/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/republish/config.hpp"

namespace roq {
namespace samples {
namespace republish {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(
      client::Symbol{
          .regex = settings_.symbol,
          .exchange = settings_.exchange,
      });
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
