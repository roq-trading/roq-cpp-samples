/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/bridge/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

using namespace std::chrono_literals;  // NOLINT

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

namespace roq {
namespace samples {
namespace bridge {
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

}  // namespace flags
}  // namespace bridge
}  // namespace samples
}  // namespace roq
