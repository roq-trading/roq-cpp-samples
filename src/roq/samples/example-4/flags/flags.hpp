/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace example_4 {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbols();
};

}  // namespace flags
}  // namespace example_4
}  // namespace samples
}  // namespace roq
