/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "utilities/time_schedule.h"

#include <cctz/civil_time.h>

#include <algorithm>
#include <sstream>

namespace examples {
namespace utilities {

namespace {
static cctz::time_zone create_time_zone(const std::string& name) {
  cctz::time_zone result;
  if (!cctz::load_time_zone(name, &result)) {
    std::stringstream ss;
    ss << "Unable to initialize time-zone object using "
      "time_zone=\"" << name << "\"";
    throw std::runtime_error(ss.str());
  }
  return result;
}
static std::vector<uint32_t> create_schedule(
    const std::vector<std::string>& schedule,
    const std::string& format) {
  std::vector<uint32_t> result;
  auto utc = cctz::utc_time_zone();
  for (const auto& iter : schedule) {
    std::chrono::system_clock::time_point time_point;
    cctz::parse(format, iter, utc, &time_point);
    auto local_time = cctz::convert(time_point, utc);
    auto seconds = local_time.hour() * 3600
                 + local_time.minute() * 60
                 + local_time.second();
    result.push_back(seconds);
  }
  return result;
}
}  // namespace

TimeSchedule::TimeSchedule(
    const std::string& time_zone,
    const std::vector<std::string>& schedule,
    const std::string& format)
    : _time_zone(create_time_zone(time_zone)),
      _schedule(create_schedule(schedule, format)) {
}

size_t TimeSchedule::get_index(
    std::chrono::system_clock::time_point time) {
  // assuming monotonically increasing time
  if (time < _next)
    return _index;
  // convert utc to local time
  auto local_time = cctz::convert(time, _time_zone);
  // local begin of day
  cctz::civil_second local_base(
      local_time.year(),
      local_time.month(),
      local_time.day());
  // total seconds since local begin of day
  auto seconds = local_time.hour() * 3600
               + local_time.minute() * 60
               + local_time.second();
  // find next schedule index
  auto iter = std::upper_bound(
      _schedule.begin(),
      _schedule.end(),
      seconds);
  if (iter == _schedule.end()) {
    _index = _schedule.size();
  } else {
    _index = iter - _schedule.begin();
  }
  // next update in local time
  auto offset = _index < _schedule.size() ? _schedule[_index] : 86400;
  auto next = local_base + offset;
  // next update in utc
  _next = cctz::convert(next, _time_zone);
  return _index;
}

}  // namespace utilities
}  // namespace examples
