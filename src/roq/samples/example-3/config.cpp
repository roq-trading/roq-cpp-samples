/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/config.h"

#include <absl/flags/flag.h>

#include "roq/samples/example-3/options.h"

namespace roq {
namespace samples {
namespace example_3 {

void Config::dispatch(Handler &handler) const {
  // accounts
  handler(client::Account{
      .regex = absl::GetFlag(FLAGS_account),
  });
  // symbols
  handler(client::Symbol{
      .regex = absl::GetFlag(FLAGS_symbol),
      .exchange = absl::GetFlag(FLAGS_exchange),
  });
  // currencies
  handler(client::Symbol{
      .regex = absl::GetFlag(FLAGS_currencies),
  });
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
