/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <cstddef>
#include <string>

namespace roq {
namespace samples {
namespace import {

// note!
// this implementation is a bit wasteful since it allocates a std::string
// on each invocation

struct Base64 final {
  static std::string encode(void const *data, size_t length);
};

}  // namespace import
}  // namespace samples
}  // namespace roq
