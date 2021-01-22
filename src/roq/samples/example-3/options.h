/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <absl/flags/declare.h>

#include <cstdint>
#include <string>

ABSL_DECLARE_FLAG(std::string, exchange);

ABSL_DECLARE_FLAG(std::string, symbol);

ABSL_DECLARE_FLAG(std::string, account);

ABSL_DECLARE_FLAG(std::string, currencies);

ABSL_DECLARE_FLAG(uint32_t, sample_freq_secs);

ABSL_DECLARE_FLAG(double, ema_alpha);

ABSL_DECLARE_FLAG(uint32_t, warmup);

ABSL_DECLARE_FLAG(bool, enable_trading);

ABSL_DECLARE_FLAG(bool, simulation);
