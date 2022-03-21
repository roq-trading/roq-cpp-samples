/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/algo-proto/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

using namespace std::literals;

ABSL_FLAG(  //
    std::string,
    config_file,
    ""s,
    "config file (path)"s);

ABSL_FLAG(  //
    std::string,
    account,
    "A1"s,
    "account"s);

ABSL_FLAG(  //
    std::string,
    strategy,
    ""s,
    "strategy"s);

ABSL_FLAG(  //
    std::string,
    side,
    "BUY"s,
    "side"s);

ABSL_FLAG(  //
    double,
    quantity,
    0.0,
    "quantity"s);

ABSL_FLAG(  //
    double,
    price,
    0.0,
    "price"s);

namespace roq {
namespace samples {
namespace algo_proto {
namespace flags {

std::string_view Flags::config_file() {
  static const std::string result{absl::GetFlag(FLAGS_config_file)};
  return result;
}

std::string_view Flags::account() {
  static const std::string result{absl::GetFlag(FLAGS_account)};
  return result;
}

std::string_view Flags::strategy() {
  static const std::string result{absl::GetFlag(FLAGS_strategy)};
  return result;
}

std::string_view Flags::side() {
  static const std::string result{absl::GetFlag(FLAGS_side)};
  return result;
}

double Flags::quantity() {
  static const double result{absl::GetFlag(FLAGS_quantity)};
  return result;
}

double Flags::price() {
  static const double result{absl::GetFlag(FLAGS_price)};
  return result;
}

}  // namespace flags
}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
