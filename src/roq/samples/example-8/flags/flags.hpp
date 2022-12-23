/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace example_8 {
namespace flags {

struct Flags final {
  static std::string_view account();
  static std::string_view exchange();
  static std::string_view symbol();
  static double quantity();
  static int32_t tick_offset();
  static bool top_of_book();
};

}  // namespace flags
}  // namespace example_8
}  // namespace samples
}  // namespace roq
