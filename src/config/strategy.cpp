/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/strategy.h"

namespace examples {
namespace config {

namespace {
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<std::string>& value) {
  stream << "[";
  bool first = true;
  for (const auto& iter : value) {
    if (!first)
      stream << ", ";
    first = false;
    stream << "\"" << iter << "\"";
  }
  return stream << "]";
}
}  // namespace

std::ostream& Strategy::write(std::ostream& stream) const {
  return stream << "{"
      "strategy_kind=\"" << strategy_kind << "\", "
      "instruments=" << instruments << ", "
      "accounts=" << accounts << ", "
      "config_file=\"" << config_file << "\""
      "}";
}

}  // namespace config
}  // namespace examples
