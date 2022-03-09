/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-2/config.hpp"

#include "roq/samples/example-2/flags.hpp"

namespace roq {
namespace samples {
namespace example_2 {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = Flags::futures_symbol(),
      .exchange = Flags::futures_exchange(),
  });
  handler(client::Symbol{
      .regex = Flags::cash_symbol(),
      .exchange = Flags::cash_exchange(),
  });
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
