/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/config.h"

#include "roq/samples/example-3/flags.h"

namespace roq {
namespace samples {
namespace example_3 {

void Config::dispatch(Handler &handler) const {
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
  });
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
