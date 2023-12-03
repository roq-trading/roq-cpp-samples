/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/io-context/config.hpp"

namespace roq {
namespace samples {
namespace io_context {

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

}  // namespace io_context
}  // namespace samples
}  // namespace roq
