/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/taker/config.h"

namespace roq {
namespace samples {
namespace taker {

Config::Config()
    : common::Config {
        .instruments = { {
          .exchange = "CFFEX",
          .symbol = "IC1906",
          .net_limit = 1,
          .tick_size = 0.01,
          .multiplier = 100.0,
          .accounts = { {
            "A1", {
              .long_limit = 10,
              .short_limit = 10,
              .long_start_of_day = 0.0,
              .short_start_of_day = 0.0,
            }
          } },
        }, {
          .exchange = "CFFEX",
          .symbol = "IF1906",
          .net_limit = 0,
          .tick_size = 0.01,
          .multiplier = 100.0,
          .accounts = {}
        }, {
          .exchange = "CFFEX",
          .symbol = "IH1906",
          .net_limit = 0,
          .tick_size = 0.01,
          .multiplier = 100.0,
          .accounts = {},
        } }
      } {
}

}  // namespace taker
}  // namespace samples
}  // namespace roq
