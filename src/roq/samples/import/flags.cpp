/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/import/flags.h"

#include <absl/flags/flag.h>

#include <string>

#include "roq/literals.h"

using namespace roq::literals;

ABSL_FLAG(  //
    std::string,
    encoding,
    "binary"_s,
    "encoding type -- one of binary or base64"_sv);

namespace roq {
namespace samples {
namespace import {

std::string_view Flags::encoding() {
  static const std::string result = absl::GetFlag(FLAGS_encoding);
  return result;
}
}  // namespace import
}  // namespace samples
}  // namespace roq
