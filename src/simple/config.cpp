/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "simple/config.h"

#include <iomanip>

namespace examples {
namespace simple {

std::ostream& operator<<(std::ostream& stream, const Config& value) {
  stream << "{"
    "config=" << value.config << ", "
    "weighted=" << (value.weighted ? "true" : "false") << ", "
    "threshold=" << value.threshold << ", "
    "quantity=" << value.quantity << ", "
    "time_zone=\"" << value.time_zone << "\", "
    "schedule=[";
  auto first = true;
  for (auto iter : value.schedule) {
    stream << (first ? "\"" : ", \"") << iter << "\"";
    first = false;
  }
  return stream << "]";
}

}  // namespace simple
}  // namespace examples
