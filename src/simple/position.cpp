/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/position.h"

#include <roq/logging.h>

namespace examples {
namespace simple {

void Position::reset() {
  _start_of_day = _new_activity = 0.0;
}

void Position::set_start_of_day(double position) {
  LOG_IF(FATAL, position < -0.0) << "Unexpected position=" << position;
  _start_of_day = position;
}

void Position::add_new_activity(double quantity) {
  LOG_IF(FATAL, quantity < -0.0) << "Unexpected quantity=" << quantity;
  _new_activity += quantity;
}

double Position::get(PositionType type) const {
  switch (type) {
    case PositionType::StartOfDay: return _start_of_day;
    case PositionType::NewActivity: return _new_activity;
    case PositionType::Current: return _start_of_day + _new_activity;
  }
}

}  // namespace simple
}  // namespace examples
