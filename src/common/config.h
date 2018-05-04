/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace examples {
namespace common {

struct Config {
  struct Instrument final {
    std::string exchange;
    std::string symbol;
    typedef std::pair<double, double> position_t;
    std::map<std::string, position_t> accounts;
    double risk_limit;  // optional
    double tick_size;  // optional
    double multiplier;  // optional
  };
  std::vector<Instrument> instruments;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);


}  // namespace common
}  // namespace examples
