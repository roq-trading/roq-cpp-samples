/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-8/config.hpp"

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // settings
  handler(client::Settings{
      .order_cancel_policy = OrderCancelPolicy::BY_ACCOUNT,
      .order_management = {},
  });
  // symbols
  handler(client::Symbol{
      .regex = settings_.symbol,
      .exchange = settings_.exchange,
  });
  // accounts
  handler(client::Account{
      .regex = settings_.account,
  });
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
