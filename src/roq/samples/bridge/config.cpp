/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/bridge/config.hpp"

#include "roq/samples/bridge/flags.hpp"

namespace roq {
namespace samples {
namespace bridge {

// === IMPLEMENTATION ===

void Config::dispatch(Handler &handler) const {
  // settings
  handler(client::Settings{
      .order_cancel_policy = OrderCancelPolicy::MANAGED_ORDERS,
      .order_management = {},
  });
  // accounts
  handler(client::Account{
      .regex = Flags::account(),
  });
  // symbols
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
  // currencies
  handler(client::Symbol{
      .regex = Flags::currencies(),
      .exchange = {},
  });
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
