/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace examples {
namespace config {

struct Strategy final {
  std::string strategy_kind;
  std::vector<std::string> instruments;
  std::vector<std::string> accounts;
  std::string config_file;

  std::ostream& write(std::ostream&) const;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Strategy& strategy) {
  return strategy.write(stream);
}

}  // namespace config
}  // namespace examples
