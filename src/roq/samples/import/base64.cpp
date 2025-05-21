/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/import/base64.hpp"

#include <cassert>
#include <cstring>
#include <iterator>

// https://stackoverflow.com/a/37109258
// https://creativecommons.org/licenses/by-sa/4.0/
// changes:
// - using reinterpret_cast (instead of C-style cast)
// - access data buffer using `unsigned char const *` (instead of `char *`)
// - using static_cast (instead of C-style cast)
// - added assertions
// - removed functions and tables not needed here
// - clang-format

namespace roq {
namespace samples {
namespace import {

// NOLINTBEGIN(readability-container-data-pointer,readability-magic-numbers)

// === CONSTANTS ===

namespace {
char const *const B64chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}

// === HELPERS ===

namespace {
std::string const b64encode(void const *data, size_t const &len) {
  std::string result((len + 2) / 3 * 4, '=');
  unsigned char const *p = reinterpret_cast<unsigned char const *>(data);
  char *str = &result[0];
  size_t j = 0, pad = len % 3;
  size_t const last = len - pad;

  for (size_t i = 0; i < last; i += 3) {
    int n = static_cast<int>(p[i]) << 16 | static_cast<int>(p[i + 1]) << 8 | static_cast<int>(p[i + 2]);
    assert(n >= 0 && n < (1 << 24));
    str[j++] = B64chars[n >> 18];
    str[j++] = B64chars[n >> 12 & 0x3F];
    str[j++] = B64chars[n >> 6 & 0x3F];
    str[j++] = B64chars[n & 0x3F];
  }
  if (pad)  /// Set padding
  {
    int n = --pad ? static_cast<int>(p[last]) << 8 | static_cast<int>(p[last + 1]) : static_cast<int>(p[last]);
    assert(n >= 0 && n < (1 << 16));
    str[j++] = B64chars[pad ? n >> 10 & 0x3F : n >> 2];
    str[j++] = B64chars[pad ? n >> 4 & 0x03F : n << 4 & 0x3F];
    str[j++] = pad ? B64chars[n << 2 & 0x3F] : '=';
  }
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

std::string Base64::encode(void const *data, size_t length) {
  auto result = b64encode(data, length);
  assert(std::size(result) == std::strlen(result.c_str()));
  return result;
}

// NOLINTEND(readability-container-data-pointer,readability-magic-numbers)

}  // namespace import
}  // namespace samples
}  // namespace roq
