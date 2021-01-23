/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace example_3 {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static std::string_view account();
  static std::string_view currencies();
  static uint32_t sample_freq_secs();
  static double ema_alpha();
  static uint32_t warmup();
  static bool enable_trading();
  static bool simulation();
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
