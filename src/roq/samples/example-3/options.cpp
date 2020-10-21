/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/options.h"

DEFINE_string(exchange, "deribit", "exchange");

DEFINE_string(symbol, "BTC-PERPETUAL", "symbol (regex)");

DEFINE_string(account, "A1", "account");

DEFINE_string(currencies, "BTC|USD", "currencies (regex)");

DEFINE_uint32(sample_freq_secs, uint32_t{1}, "sample frequency (seconds)");

DEFINE_double(
    ema_alpha,
    double{0.33},
    "alpha used to compute exponential moving average (0 < alpha <= 1)");
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

DEFINE_uint32(
    warmup,
    uint32_t{120},
    "warmup (number of samples before a signal is generated)");

DEFINE_bool(enable_trading, false, "trading must explicitly be enabled!");

DEFINE_bool(simulation, false, "requires an event-log");
