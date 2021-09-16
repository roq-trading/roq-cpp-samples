/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-7/flags/flags.h"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    label,
    "ToB",
    "label");

ABSL_FLAG(  //
    std::string,
    account,
    "",
    "account name");

ABSL_FLAG(  //
    std::string,
    exchange,
    "deribit",
    "exchange name");

ABSL_FLAG(  //
    std::string,
    symbol,
    "BTC-PERPETUAL",
    "symbol");

ABSL_FLAG(  //
    bool,
    simulation,
    false,
    "requires an event-log");

namespace roq {
namespace samples {
namespace example_7 {
namespace flags {

std::string_view Flags::label() {
  static const std::string result = absl::GetFlag(FLAGS_label);
  return result;
}

std::string_view Flags::account() {
  static const std::string result = absl::GetFlag(FLAGS_account);
  return result;
}

std::string_view Flags::exchange() {
  static const std::string result = absl::GetFlag(FLAGS_exchange);
  return result;
}

std::string_view Flags::symbol() {
  static const std::string result = absl::GetFlag(FLAGS_symbol);
  return result;
}

bool Flags::simulation() {
  static const bool result = absl::GetFlag(FLAGS_simulation);
  return result;
}

}  // namespace flags
}  // namespace example_7
}  // namespace samples
}  // namespace roq
