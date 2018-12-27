/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <chrono>
#include <string>
#include <vector>

namespace examples {
namespace utilities {

// finds the index matching current time-period respecting a  time-zone
class TimeSchedule final {
 public:
  explicit TimeSchedule(
      const std::string& time_zone,
      const std::vector<std::string>& schedule,
      const std::string& format = "%H:%M");
  // returns the index of the *next* time point in the schedule
  // note! the index equals the size of the schedule when time is
  // between the last entry and midnight
  size_t get_index(std::chrono::system_clock::time_point time);

 private:
  const cctz::time_zone _time_zone;
  const std::vector<uint32_t> _schedule;
  mutable std::chrono::system_clock::time_point _next;
  mutable size_t _index = 0;
};

}  // namespace utilities
}  // namespace examples
