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
    double tick_size;  // optional
    double multiplier;  // optional
    double net_limit;
    struct Account final {
      double long_limit;
      double short_limit;
      double long_start_of_day;
      double short_start_of_day;
    };
    std::map<std::string, Account> accounts;  // optional
  };
  std::vector<Instrument> instruments;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);


}  // namespace common
}  // namespace examples
