/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <cstdint>
#include <string_view>

namespace roq {
namespace samples {
namespace republish {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static uint16_t port();
};

}  // namespace flags
}  // namespace republish
}  // namespace samples
}  // namespace roq
