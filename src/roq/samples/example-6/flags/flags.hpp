/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>

namespace roq {
namespace samples {
namespace example_6 {
namespace flags {

struct Flags final {
  static std::string_view source_name_1();
  static std::string_view exchange_1();
  static std::string_view symbol_1();
  static std::string_view source_name_2();
  static std::string_view exchange_2();
  static std::string_view symbol_2();
  static std::string_view account();
  static std::string_view currencies();
  static std::chrono::nanoseconds sample_freq();
  static double ema_alpha();
  static uint32_t warmup();
  static bool enable_trading();
  static bool simulation();
};

}  // namespace flags
}  // namespace example_6
}  // namespace samples
}  // namespace roq
