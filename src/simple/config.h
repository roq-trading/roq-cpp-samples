/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>
#include <ostream>
#include <string>
#include <vector>

#include "common/config.h"

namespace examples {
namespace simple {

struct Config final {
  common::Config config;
  bool weighted;
  double threshold;
  double quantity;
  std::string time_zone;
  std::vector<std::string> schedule;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);

}  // namespace simple
}  // namespace examples
