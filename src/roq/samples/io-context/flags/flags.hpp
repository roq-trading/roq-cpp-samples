/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace samples {
namespace io_context {
namespace flags {

struct Flags final {
  static std::string_view exchange();
  static std::string_view symbol();
  static uint16_t ws_port();
  static uint16_t udp_port();
};

}  // namespace flags
}  // namespace io_context
}  // namespace samples
}  // namespace roq
