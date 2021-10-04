/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-8/config.h"

#include "roq/samples/example-8/flags.h"

namespace roq {
namespace samples {
namespace example_8 {

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
