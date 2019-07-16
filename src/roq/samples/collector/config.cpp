/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/collector/config.h"

namespace roq {
namespace samples {
namespace collector {

void Config::dispatch(Handler& handler) const {
  handler.on(
      client::Symbol {
        .exchange = "CFFEX",
        .regex = ".*",
      });
}

}  // namespace collector
}  // namespace samples
}  // namespace roq
