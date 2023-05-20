/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace python {
namespace flags {

struct Flags final {
  static std::string_view deribit_exchange();
  static std::string_view deribit_symbols();
  static std::string_view coinbase_pro_exchange();
  static std::string_view coinbase_pro_symbols();
};

}  // namespace flags
}  // namespace python
}  // namespace samples
}  // namespace roq
