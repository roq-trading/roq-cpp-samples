/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-4/config.h"

#include "roq/samples/example-4/flags.h"

namespace roq {
namespace samples {
namespace example_4 {

void Config::dispatch(Handler &handler) const {
  handler(client::Symbol{
      .regex = Flags::symbols(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
