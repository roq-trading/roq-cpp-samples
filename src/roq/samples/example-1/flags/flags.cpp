/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-1/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    deribit_exchange,
    "deribit",
    "name of the deribit exchange");

ABSL_FLAG(  //
    std::string,
    deribit_symbols,
    "BTC-\\d{2}\\D{3}\\d{2}",  // e.g. "BTC-27MAR20"
    "regex used to subscribe deribit symbols");

ABSL_FLAG(  //
    std::string,
    coinbase_pro_exchange,
    "coinbase-pro",
    "name of the coinbase-pro exchange");

ABSL_FLAG(  //
    std::string,
    coinbase_pro_symbols,
    "BTC-.*",  // e.g. "BTC-USD"
    "regex used to subscribe coinbase-pro symbols");

namespace roq {
namespace samples {
namespace example_1 {
namespace flags {

std::string_view Flags::deribit_exchange() {
  static const std::string result = absl::GetFlag(FLAGS_deribit_exchange);
  return result;
}

std::string_view Flags::deribit_symbols() {
  static const std::string result = absl::GetFlag(FLAGS_deribit_symbols);
  return result;
}

std::string_view Flags::coinbase_pro_exchange() {
  static const std::string result = absl::GetFlag(FLAGS_coinbase_pro_exchange);
  return result;
}

std::string_view Flags::coinbase_pro_symbols() {
  static const std::string result = absl::GetFlag(FLAGS_coinbase_pro_symbols);
  return result;
}

}  // namespace flags
}  // namespace example_1
}  // namespace samples
}  // namespace roq
