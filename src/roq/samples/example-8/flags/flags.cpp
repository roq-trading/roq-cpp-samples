/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-8/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    account,
    "A1",
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
    "symbol name");

ABSL_FLAG(  //
    double,
    quantity,
    1.0,
    "quantity");

ABSL_FLAG(  //
    int32_t,
    tick_offset,
    100,
    "offset as number of ticks");

ABSL_FLAG(  //
    bool,
    top_of_book,
    false,
    "use top of book as trigger?");

namespace roq {
namespace samples {
namespace example_8 {
namespace flags {

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

double Flags::quantity() {
  static double const result = absl::GetFlag(FLAGS_quantity);
  return result;
}

int32_t Flags::tick_offset() {
  static const int32_t result = absl::GetFlag(FLAGS_tick_offset);
  return result;
}

bool Flags::top_of_book() {
  static bool const result = absl::GetFlag(FLAGS_top_of_book);
  return result;
}

}  // namespace flags
}  // namespace example_8
}  // namespace samples
}  // namespace roq
