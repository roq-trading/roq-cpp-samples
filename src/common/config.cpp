/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/config.h"

  struct Instrument final {
    std::string exchange;
    std::string symbol;
    typedef std::pair<double, double> position_t;
    std::map<std::string, position_t> accounts;
    double risk_limit;  // optional
    double tick_size;  // optional
  };

namespace examples {
namespace common {

namespace {
std::ostream& operator<<(
    std::ostream& stream,
    const std::map<std::string, std::pair<double, double> >& accounts) {
  stream << "{";
  bool first = true;
  for (const auto& iter : accounts) {
    if (first == false)
      stream << ", ";
    first = false;
    stream << "\"" << iter.first << "\"={"
      "long=" << iter.second.first << ", "
      "short=" << iter.second.second <<
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
    "accounts=" << instrument.accounts << ", "
    "risk_limit=" << instrument.risk_limit << ", "
    "tick_size=" << instrument.tick_size <<
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
