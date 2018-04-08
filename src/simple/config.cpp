/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/config.h"

namespace examples {
namespace simple {

std::ostream& operator<<(std::ostream& stream, const Config& value) {
  return stream << "{"
    "exchange=\"" << value.exchange << "\", "
    "instrument=\"" << value.instrument << "\", "
    "threshold=" << value.threshold << ", "
    "quantity=" << value.quantity <<
    "}";
}

}  // namespace simple
}  // namespace examples
