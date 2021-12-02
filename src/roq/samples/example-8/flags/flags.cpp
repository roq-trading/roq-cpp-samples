/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-8/flags/flags.h"

#include <absl/flags/flag.h>

#include <string>

using namespace std::chrono_literals;  // NOLINT

namespace {
class TimePeriod final {
 public:
  TimePeriod() = default;

  TimePeriod(const std::chrono::nanoseconds value) : value_(absl::FromChrono(value)) {}  // NOLINT (allow implicit)

  operator const absl::Duration &() const { return value_; }

  static std::string unparse(const TimePeriod &flag) { return absl::AbslUnparseFlag(flag.value_); }

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
static std::string AbslUnparseFlag(T flag) {
  return T::unparse(flag);
}

template <typename T>
static bool AbslParseFlag(absl::string_view text, T *flag, std::string *error) {
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
    "symbol");

ABSL_FLAG(  //
    uint32_t,
    tick_offset,
    5,
    "offset against best price (tick_size units)");

ABSL_FLAG(  //
    TimePeriod,
    wait_time,
    {5s},
    "wait time before test is initiated");

namespace roq {
namespace samples {
namespace example_8 {
namespace flags {

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

uint32_t Flags::tick_offset() {
  static const uint32_t result = absl::GetFlag(FLAGS_tick_offset);
  return result;
}

std::chrono::nanoseconds Flags::wait_time() {
  static const std::chrono::nanoseconds result{absl::ToChronoNanoseconds(absl::GetFlag(FLAGS_wait_time))};
  return result;
}

}  // namespace flags
}  // namespace example_8
}  // namespace samples
}  // namespace roq
