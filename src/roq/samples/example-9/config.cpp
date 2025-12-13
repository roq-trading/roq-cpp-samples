/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-9/config.hpp"

namespace roq {
namespace samples {
namespace example_9 {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  auto helper = [&](auto &exchange, auto &symbol) {
    auto subscription = client::Symbol{
        .regex = symbol,
        .exchange = exchange,
    };
    handler(subscription);
  };
  helper(settings_.exchange, settings_.symbol);
}

}  // namespace example_9
}  // namespace samples
}  // namespace roq
