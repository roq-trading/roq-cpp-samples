/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/polling/config.hpp"

namespace roq {
namespace samples {
namespace polling {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  handler(
      client::Settings{
          .order_cancel_policy = OrderCancelPolicy::BY_ACCOUNT,
          .order_management = {},
      });
  handler(
      client::Account{
          .regex = settings_.account,
      });
  handler(
      client::Symbol{
          .regex = settings_.symbol,
          .exchange = settings_.exchange,
      });
}

}  // namespace polling
}  // namespace samples
}  // namespace roq
