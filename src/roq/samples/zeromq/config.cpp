/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/zeromq/config.hpp"

#include "roq/samples/zeromq/flags.hpp"

namespace roq {
namespace samples {
namespace zeromq {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace zeromq
}  // namespace samples
}  // namespace roq
