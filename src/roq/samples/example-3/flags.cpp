/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/flags.h"

#include <absl/flags/flag.h>

#include <string>

using namespace std::literals;  // NOLINT

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit"s,
    "exchange"sv);

ABSL_FLAG(  //
    std::string,
    symbol,
    "BTC-PERPETUAL"s,
    "symbol (regex)"sv);

ABSL_FLAG(  //
    std::string,
    account,
    "A1"s,
    "account"sv);

ABSL_FLAG(  //
    std::string,
    currencies,
    "BTC|USD"s,
    "currencies (regex)"sv);

ABSL_FLAG(  //
    uint32_t,
    sample_freq_secs,
    uint32_t{1},
    "sample frequency (seconds)"sv);

ABSL_FLAG(  //
    double,
    ema_alpha,
    double{0.33},
    "alpha used to compute exponential moving average (0 < alpha <= 1)"sv);
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

ABSL_FLAG(  //
    uint32_t,
    warmup,
    uint32_t{120},
    "warmup (number of samples before a signal is generated)"sv);

ABSL_FLAG(  //
    bool,
    enable_trading,
    false,
    "trading must explicitly be enabled!"sv);

ABSL_FLAG(  //
    bool,
    simulation,
    false,
    "requires an event-log"sv);

namespace roq {
namespace samples {
namespace example_3 {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

std::string_view Flags::account() {
  static const std::string result = absl::GetFlag(FLAGS_account);
  return result;
}

std::string_view Flags::currencies() {
  static const std::string result = absl::GetFlag(FLAGS_currencies);
  return result;
}

uint32_t Flags::sample_freq_secs() {
  static const uint32_t result = absl::GetFlag(FLAGS_sample_freq_secs);
  return result;
}

double Flags::ema_alpha() {
  static const double result = absl::GetFlag(FLAGS_ema_alpha);
  return result;
}

uint32_t Flags::warmup() {
  static const uint32_t result = absl::GetFlag(FLAGS_warmup);
  return result;
}

bool Flags::enable_trading() {
  static const bool result = absl::GetFlag(FLAGS_enable_trading);
  return result;
}

bool Flags::simulation() {
  static const bool result = absl::GetFlag(FLAGS_simulation);
  return result;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
