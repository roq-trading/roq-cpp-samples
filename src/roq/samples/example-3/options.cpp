/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/options.h"

#include <absl/flags/flag.h>

ABSL_FLAG(std::string, exchange, "deribit", "exchange");

ABSL_FLAG(std::string, symbol, "BTC-PERPETUAL", "symbol (regex)");

ABSL_FLAG(std::string, account, "A1", "account");

ABSL_FLAG(std::string, currencies, "BTC|USD", "currencies (regex)");

ABSL_FLAG(
    uint32_t, sample_freq_secs, uint32_t{1}, "sample frequency (seconds)");

ABSL_FLAG(
    double,
    ema_alpha,
    double{0.33},
    "alpha used to compute exponential moving average (0 < alpha <= 1)");
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

ABSL_FLAG(
    uint32_t,
    warmup,
    uint32_t{120},
    "warmup (number of samples before a signal is generated)");

ABSL_FLAG(bool, enable_trading, false, "trading must explicitly be enabled!");

ABSL_FLAG(bool, simulation, false, "requires an event-log");
