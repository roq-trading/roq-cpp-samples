/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <string>

namespace roq {
namespace samples {
namespace import {

// note!
// this implementation is a bit wasteful since it allocates a std::string
// on each invocation

class Base64 final {
 public:
  static std::string encode(void const *data, size_t length);
};

}  // namespace import
}  // namespace samples
}  // namespace roq
