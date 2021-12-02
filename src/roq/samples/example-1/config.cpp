/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-1/config.h"

#include "roq/samples/example-1/flags.h"

namespace roq {
namespace samples {
namespace example_1 {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = Flags::deribit_symbols(),
      .exchange = Flags::deribit_exchange(),
  });
  handler(client::Symbol{
      .regex = Flags::coinbase_pro_symbols(),
      .exchange = Flags::coinbase_pro_exchange(),
  });
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
