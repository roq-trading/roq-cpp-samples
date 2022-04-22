/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace zeromq {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static uint16_t port();
};

}  // namespace flags
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
