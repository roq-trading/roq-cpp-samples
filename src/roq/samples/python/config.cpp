/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/python/config.hpp"

namespace roq {
namespace samples {
namespace python {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.symbols,
      .exchange = settings_.exchange,
  });
}

}  // namespace python
}  // namespace samples
}  // namespace roq
