/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace example_5 {
namespace flags {

struct Flags final {
  static std::string_view deribit_exchange();
  static std::string_view deribit_symbols();
  static std::string_view coinbase_pro_exchange();
  static std::string_view coinbase_pro_symbols();
};

}  // namespace flags
}  // namespace example_5
}  // namespace samples
}  // namespace roq
