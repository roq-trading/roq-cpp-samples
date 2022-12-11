/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace algo_proto {
namespace flags {

struct Flags final {
  static std::string_view config_file();
  static std::string_view account();
  static std::string_view strategy();
  static std::string_view side();
  static double quantity();
  static double price();
};

}  // namespace flags
}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
