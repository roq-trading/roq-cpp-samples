/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/config.h"

#include <iomanip>

namespace examples {
namespace simple {

std::ostream& operator<<(std::ostream& stream, const Config& value) {
  stream << "{"
    "config=" << value.config << ", "
    "weighted=" << (value.weighted ? "true" : "false") << ", "
    "threshold=" << value.threshold << ", "
    "quantity=" << value.quantity;

  stream << "timers = [";
  for (auto timer : value.timers) {
    auto t = std::chrono::system_clock::to_time_t(timer.time);
    stream << "{"
              "event=" << timer.event << ", "
              "time=" << std::put_time(std::localtime(&t), "%T") << ", "
              "arguments=" << timer.arguments << ","
              "enabled=" << (timer.enabled ? "true" : "false") <<
              "},";
  }
  return stream << "]";
}

}  // namespace simple
}  // namespace examples
