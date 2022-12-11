/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/import/flags/flags.hpp"

#include <absl/flags/flag.h>

#include <string>

ABSL_FLAG(  //
    std::string,
    encoding,
    "binary",
    "encoding type -- one of binary or base64");

namespace roq {
namespace samples {
namespace import {
namespace flags {

std::string_view Flags::encoding() {
  static const std::string result = absl::GetFlag(FLAGS_encoding);
  return result;
}

}  // namespace flags
}  // namespace import
}  // namespace samples
}  // namespace roq
