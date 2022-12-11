/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-4/config.hpp"

#include "roq/samples/example-4/flags.hpp"

namespace roq {
namespace samples {
namespace example_4 {

// === IMPLEMENTATION ===

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = Flags::symbols(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
