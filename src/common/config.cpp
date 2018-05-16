/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/config.h"

namespace examples {
namespace common {

namespace {
std::ostream& operator<<(
    std::ostream& stream,
    const std::map<std::string, Config::Instrument::Account>& accounts) {
  stream << "{";
  bool first = true;
  for (const auto& iter : accounts) {
    if (first == false)
      stream << ", ";
    first = false;
    stream << "\"" << iter.first << "\"={"
      "long_limit=" << iter.second.long_limit << ", "
      "short_limit=" << iter.second.short_limit << ", "
      "long_start_of_day=" << iter.second.long_start_of_day << ", "
      "short_start_of_day=" << iter.second.short_start_of_day <<
      "}";
  }
  return stream << "}";
}
std::ostream& operator<<(
    std::ostream& stream,
    const Config::Instrument& instrument) {
  return stream << "{"
    "exchange=\"" << instrument.exchange << "\", "
    "symbol=\"" << instrument.symbol << "\", "
    "tick_size=" << instrument.tick_size << ", "
    "multiplier=" << instrument.multiplier << ", "
    "accounts=" << instrument.accounts <<
    "}";
}
}  // namespace

std::ostream& operator<<(std::ostream& stream, const Config& value) {
  stream << "[";
  bool first = true;
  for (const auto& iter : value.instruments) {
    if (first == false)
      stream << ", ";
    first = false;
    stream << iter;
  }
  return stream << "]";
}

}  // namespace common
}  // namespace examples
