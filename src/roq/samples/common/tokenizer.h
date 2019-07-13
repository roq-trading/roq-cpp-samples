/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace roq {
namespace samples {
namespace common {

struct Tokenizer final {
  template <typename F>
  static void parse(
      const std::string_view& line,
      const std::string_view& delimiters,
      F callback) {
    if (line.empty())
      return;
    auto size = line.size();
    std::string_view::size_type begin = 0;
    while (true) {
      auto end = line.find_first_of(delimiters, begin);
      auto length = ((end == std::string_view::npos) ? size : end) - begin;
      callback(length ? line.data() + begin : nullptr, length);
      if (end == std::string_view::npos)
        break;
      begin = end + 1;
    }
  }
};

inline auto tokenize(
    const std::string_view& line,
    const std::string_view& delimiters) {
  std::vector<std::string> result;
  Tokenizer::parse(
      line,
      delimiters,
      [&result](auto text, auto length) {
        result.emplace_back(text, length);
      });
  return result;
}

}  // namespace common
}  // namespace samples
}  // namespace roq
