/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-1/config.hpp"

namespace roq {
namespace samples {
namespace example_1 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = settings_.deribit_symbols,
      .exchange = settings_.deribit_exchange,
  });
  handler(client::Symbol{
      .regex = settings_.coinbase_pro_symbols,
      .exchange = settings_.coinbase_pro_exchange,
  });
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
