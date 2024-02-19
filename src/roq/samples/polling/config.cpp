/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/polling/config.hpp"

namespace roq {
namespace samples {
namespace polling {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = settings_.symbol,
      .exchange = settings_.exchange,
  });
}

}  // namespace polling
}  // namespace samples
}  // namespace roq
