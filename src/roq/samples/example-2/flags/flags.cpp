/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-2/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    futures_exchange,
    "deribit",
    "futures exchange");

ABSL_FLAG(  //
    std::string,
    futures_symbol,
    "BTC-PERPETUAL",
    "futures symbol");

ABSL_FLAG(  //
    std::string,
    cash_exchange,
    "coinbase-pro",
    "cash exchange");

ABSL_FLAG(  //
    std::string,
    cash_symbol,
    "BTC-USD",
    "cash symbol");

ABSL_FLAG(  //
    double,
    alpha,
    0.2,
    "alpha used to compute exponential moving average");

namespace roq {
namespace samples {
namespace example_2 {
namespace flags {

std::string_view Flags::futures_exchange() {
  static const std::string result = absl::GetFlag(FLAGS_futures_exchange);
  return result;
}

std::string_view Flags::futures_symbol() {
  static const std::string result = absl::GetFlag(FLAGS_futures_symbol);
  return result;
}

std::string_view Flags::cash_exchange() {
  static const std::string result = absl::GetFlag(FLAGS_cash_exchange);
  return result;
}

std::string_view Flags::cash_symbol() {
  static const std::string result = absl::GetFlag(FLAGS_cash_symbol);
  return result;
}

double Flags::alpha() {
  static const double result = absl::GetFlag(FLAGS_alpha);
  return result;
}

}  // namespace flags
}  // namespace example_2
}  // namespace samples
}  // namespace roq
