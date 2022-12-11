/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/io-context/config.hpp"

#include "roq/samples/io-context/flags.hpp"

namespace roq {
namespace samples {
namespace io_context {

// === IMPLEMENTATION ===

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
