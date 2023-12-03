/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-2/config.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.futures_symbol,
      .exchange = settings_.futures_exchange,
  });
  handler(client::Symbol{
      .regex = settings_.cash_symbol,
      .exchange = settings_.cash_exchange,
  });
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
