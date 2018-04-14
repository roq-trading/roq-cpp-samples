/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <vector>

namespace examples {
namespace common {

struct Config {
  struct Instrument final {
    std::string exchange;
    std::string instrument;
    double risk_limit;  // optional
    double long_position;  // optional, start of day
    double short_position;  // optional, start of day
    double tick_size;  // optional
  };
  std::vector<Instrument> instruments;
};


}  // namespace common
}  // namespace examples
