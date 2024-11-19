/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/bridge/config.hpp"

namespace roq {
namespace samples {
namespace bridge {

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
      .regex = settings_.symbol,
      .exchange = settings_.exchange,
  });
  // currencies
  handler(client::Symbol{
      .regex = settings_.currencies,
      .exchange = {},
  });
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
