/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/republish/config.hpp"

#include "roq/samples/republish/flags.hpp"

namespace roq {
namespace samples {
namespace republish {

void Config::dispatch(Handler &handler) const {
  // callback for each subscription pattern
  handler(client::Symbol{
      .regex = Flags::symbol(),
      .exchange = Flags::exchange(),
  });
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
