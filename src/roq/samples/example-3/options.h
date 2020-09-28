/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

DECLARE_string(exchange);

DECLARE_string(symbol);

DECLARE_string(account);

DECLARE_string(currencies);

DECLARE_uint32(sample_freq_secs);

DECLARE_double(ema_alpha);

DECLARE_uint32(warmup);

DECLARE_bool(enable_trading);

DECLARE_bool(simulation);
