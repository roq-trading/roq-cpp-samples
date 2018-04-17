/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/position.h"

#include <roq/logging.h>

namespace examples {
namespace common {

Position::Position(double position)
    : _manual(position),
      _use_position_update(std::isnan(position)),
      _start_of_day(_use_position_update ? 0.0 : _manual) {
  LOG_IF(FATAL, _manual < 0.0) << "Unexpected";
}

void Position::reset() {
  _last_order_id = 0;
  _start_of_day = _use_position_update ? 0.0 : _manual;
  _new_activity = 0.0;
}

double Position::get(PositionType type) const {
  switch (type) {
    case PositionType::StartOfDay: return _start_of_day;
    case PositionType::NewActivity: return _new_activity;
    case PositionType::Current: return _start_of_day + _new_activity;
  }
}

void Position::set_start_of_day(uint32_t last_order_id, double position) {
  LOG_IF(FATAL, position < -0.0) << "Unexpected position=" << position;
  _last_order_id = last_order_id;
  if (_use_position_update)
    _start_of_day = position;
}

void Position::add_new_activity(uint32_t order_id, double quantity) {
  LOG_IF(FATAL, quantity < -0.0) << "Unexpected quantity=" << quantity;
  if (_last_order_id < order_id)
    _new_activity += quantity;
}

std::ostream& Position::write(std::ostream& stream) const {
  return stream << "{"
    "start_of_day=" << get(PositionType::StartOfDay) << ", "
    "new_activity=" << get(PositionType::NewActivity) << ", "
    "current=" << get(PositionType::Current) << ", "
    "last_order_id=" << _last_order_id <<
    "}";
}

}  // namespace common
}  // namespace examples
