/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>
#include <chrono>

#include "common/config.h"

namespace examples {
namespace simple {

struct SchedulerTimer final {
    std::string event;
    std::chrono::system_clock::time_point time;
    int arguments;
    bool enabled;
};

struct Config final {
  common::Config config;
  bool weighted;
  double threshold;
  double quantity;
  std::vector<SchedulerTimer> timers;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);

}  // namespace simple
}  // namespace examples
