/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace metrics {
namespace flags {

struct Flags final {
  static std::string_view symbols();
};

}  // namespace flags
}  // namespace metrics
}  // namespace samples
}  // namespace roq
