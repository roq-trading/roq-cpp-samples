/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/config.h"

#include "roq/samples/example-3/options.h"

namespace roq {
namespace samples {
namespace example_3 {

void Config::dispatch(Handler &handler) const {
  // accounts
  handler(client::Account {
      .regex = FLAGS_account,
  });
  // symbols
  handler(client::Symbol {
      .regex = FLAGS_symbol,
      .exchange = FLAGS_exchange,
  });
  // currencies
  handler(client::Symbol {
      .regex = FLAGS_currencies,
  });
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
