/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/python/config.hpp"

#include "roq/samples/python/flags/flags.hpp"

namespace roq {
namespace samples {
namespace python {

// === IMPLEMENTATION ===

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = flags::Flags::deribit_symbols(),
      .exchange = flags::Flags::deribit_exchange(),
  });
  handler(client::Symbol{
      .regex = flags::Flags::coinbase_pro_symbols(),
      .exchange = flags::Flags::coinbase_pro_exchange(),
  });
}

}  // namespace python
}  // namespace samples
}  // namespace roq
