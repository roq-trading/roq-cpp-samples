/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-7/config.h"

#include "roq/samples/example-7/flags.h"

namespace roq {
namespace samples {
namespace example_7 {

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
