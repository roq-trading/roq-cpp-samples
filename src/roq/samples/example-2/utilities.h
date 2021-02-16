/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <cmath>
#include <type_traits>

namespace roq {
namespace samples {
namespace example_2 {

constexpr auto TOLERANCE = 1.0e-10;

// update if value has changed
template <typename T>
inline bool update(T &lhs, const T &rhs) {
  if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
    if (std::fabs(lhs - rhs) < TOLERANCE)
      return false;
  } else {
    if (lhs == rhs)
      return false;
  }
  lhs = rhs;
  return true;
}
// check if value is zero
template <typename T>
inline bool is_zero(const T &value) {
  if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
    return std::fabs(value) < TOLERANCE;
  } else {
    return value == T{};
  }
}
// check if value is stricly positive
template <typename T>
inline bool is_strictly_positive(const T &value) {
  if constexpr (std::is_floating_point_v<std::decay_t<T>>) {
    return !(value < TOLERANCE);
  } else {
    return value > T{};
  }
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
