/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace example_7 {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static bool simulation();
};

}  // namespace flags
}  // namespace example_7
}  // namespace samples
}  // namespace roq
