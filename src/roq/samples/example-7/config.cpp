/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-7/config.hpp"

#include "roq/samples/example-7/flags.hpp"

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
