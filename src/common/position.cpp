/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/position.h"

#include <roq/logging.h>

namespace examples {
namespace common {

void Position::reset() {
  _start_of_day = _new_activity = 0.0;
}

double Position::get(PositionType type) const {
  switch (type) {
    case PositionType::StartOfDay: return _start_of_day;
    case PositionType::NewActivity: return _new_activity;
    case PositionType::Current: return _start_of_day + _new_activity;
    case PositionType::Reference: return _reference;
  }
}

void Position::set_start_of_day(double position) {
  LOG_IF(FATAL, position < -0.0) << "Unexpected position=" << position;
  _start_of_day = position;
}

void Position::add_new_activity(double quantity) {
  LOG_IF(FATAL, quantity < -0.0) << "Unexpected quantity=" << quantity;
  _new_activity += quantity;
}

void Position::set_reference(double position) {
  LOG_IF(FATAL, position < -0.0) << "Unexpected position=" << position;
  _reference = position;
}

std::ostream& operator<<(std::ostream& stream, const Position& position) {
  return stream << "{"
    "start_of_day=" << position.get(PositionType::StartOfDay) << ", "
    "new_activity=" << position.get(PositionType::NewActivity) << ", "
    "current=" << position.get(PositionType::Current) << ", "
    "reference=" << position.get(PositionType::Reference) <<
    "}";
}

}  // namespace common
}  // namespace examples
