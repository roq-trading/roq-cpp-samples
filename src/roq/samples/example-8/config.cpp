/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-8/config.hpp"

#include "roq/samples/example-8/flags.hpp"

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

void Config::dispatch(Handler &handler) const {
  // settings
  handler(client::Settings{
      .order_cancel_policy = OrderCancelPolicy::BY_ACCOUNT,
      .order_management = {},
  });
  // symbols
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
  // accounts
  handler(client::Account{
      .regex = Flags::account(),
  });
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
