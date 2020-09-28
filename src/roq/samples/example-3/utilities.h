/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

namespace roq {
namespace samples {
namespace example_3 {

template <typename T>
inline bool update(T& lhs, const T& rhs) {
  if (lhs == rhs)  // note! too simplistic for T == double
    return false;
  lhs = rhs;
  return true;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
