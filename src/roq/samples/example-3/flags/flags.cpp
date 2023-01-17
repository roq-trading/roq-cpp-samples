/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-3/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

using namespace std::chrono_literals;  // NOLINT

namespace {
struct TimePeriod final {
  TimePeriod() = default;

  TimePeriod(const std::chrono::nanoseconds value) : value_(absl::FromChrono(value)) {}  // NOLINT (allow implicit)

  operator absl::Duration const &() const { return value_; }

  static std::string unparse(TimePeriod const &flag) { return absl::AbslUnparseFlag(flag.value_); }

  static bool parse(absl::string_view text, TimePeriod *flag, std::string *error) {
    if (!absl::ParseFlag(text, &flag->value_, error)) {
      return false;
    }
    return true;
  }

 private:
  absl::Duration value_ = {};
};

template <typename T>
std::string AbslUnparseFlag(T flag) {
  return T::unparse(flag);
}

template <typename T>
bool AbslParseFlag(absl::string_view text, T *flag, std::string *error) {
  return T::parse(text, flag, error);
}
}  // namespace

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange");

ABSL_FLAG(  //
    std::string,
    symbol,
    "BTC-PERPETUAL",
    "symbol (regex)");

ABSL_FLAG(  //
    std::string,
    account,
    "A1",
    "account");

ABSL_FLAG(  //
    std::string,
    currencies,
    "BTC|USD",
    "currencies (regex)");

ABSL_FLAG(  //
    TimePeriod,
    sample_freq,
    {1s},
    "sample frequency");

ABSL_FLAG(  //
    double,
    ema_alpha,
    0.33,
    "alpha used to compute exponential moving average (0 < alpha <= 1)");
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

ABSL_FLAG(  //
    uint32_t,
    warmup,
    120,
    "warmup (number of samples before a signal is generated)");

ABSL_FLAG(  //
    bool,
    enable_trading,
    false,
    "trading must explicitly be enabled!");

ABSL_FLAG(  //
    bool,
    simulation,
    false,
    "requires an event-log");

namespace roq {
namespace samples {
namespace example_3 {
namespace flags {

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

std::chrono::nanoseconds Flags::sample_freq() {
  static const std::chrono::nanoseconds result{absl::ToChronoNanoseconds(absl::GetFlag(FLAGS_sample_freq))};
  return result;
}

double Flags::ema_alpha() {
  static double const result = absl::GetFlag(FLAGS_ema_alpha);
  return result;
}

uint32_t Flags::warmup() {
  static const uint32_t result = absl::GetFlag(FLAGS_warmup);
  return result;
}

bool Flags::enable_trading() {
  static bool const result = absl::GetFlag(FLAGS_enable_trading);
  return result;
}

bool Flags::simulation() {
  static bool const result = absl::GetFlag(FLAGS_simulation);
  return result;
}

}  // namespace flags
}  // namespace example_3
}  // namespace samples
}  // namespace roq
