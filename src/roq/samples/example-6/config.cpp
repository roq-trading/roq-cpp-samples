/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-6/config.hpp"

namespace roq {
namespace samples {
namespace example_6 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  // settings
  handler(client::Settings{
      .order_cancel_policy = OrderCancelPolicy::MANAGED_ORDERS,
      .order_management = {},
  });
  // accounts
  handler(client::Account{
      .regex = settings_.account,
  });
  // symbols
  handler(client::Symbol{
      .regex = settings_.symbol_1,
      .exchange = settings_.exchange_1,
  });
  handler(client::Symbol{
      .regex = settings_.symbol_2,
      .exchange = settings_.exchange_2,
  });
  // currencies
  handler(client::Symbol{
      .regex = settings_.currencies,
      .exchange = {},
  });
}

}  // namespace example_6
}  // namespace samples
}  // namespace roq
