/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-5/config.hpp"

#include "roq/samples/example-5/flags.hpp"

namespace roq {
namespace samples {
namespace example_5 {

// === IMPLEMENTATION ===

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

}  // namespace example_5
}  // namespace samples
}  // namespace roq
