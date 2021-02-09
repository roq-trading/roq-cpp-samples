/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-1/flags.h"

#include <absl/flags/flag.h>

#include <string>

using namespace std::literals;  // NOLINT

ABSL_FLAG(  //
    std::string,
    deribit_exchange,
    "deribit"s,
    "name of the deribit exchange"sv);

ABSL_FLAG(  //
    std::string,
    deribit_symbols,
    "BTC-\\d{2}\\D{3}\\d{2}"s,  // e.g. "BTC-27MAR20"
    "regex used to subscribe deribit symbols"sv);

ABSL_FLAG(  //
    std::string,
    coinbase_pro_exchange,
    "coinbase-pro"s,
    "name of the coinbase-pro exchange"sv);

ABSL_FLAG(  //
    std::string,
    coinbase_pro_symbols,
    "BTC-.*"s,  // e.g. "BTC-USD"
    "regex used to subscribe coinbase-pro symbols"sv);

namespace roq {
namespace samples {
namespace example_1 {

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

}  // namespace example_1
}  // namespace samples
}  // namespace roq
