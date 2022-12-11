/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace import {
namespace flags {

struct Flags final {
  static std::string_view encoding();
};

}  // namespace flags
}  // namespace import
}  // namespace samples
}  // namespace roq
