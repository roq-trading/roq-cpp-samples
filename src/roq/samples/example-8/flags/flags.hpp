/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>

namespace roq {
namespace samples {
namespace example_8 {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static uint32_t tick_offset();
  static std::chrono::nanoseconds wait_time();
};

}  // namespace flags
}  // namespace example_8
}  // namespace samples
}  // namespace roq
