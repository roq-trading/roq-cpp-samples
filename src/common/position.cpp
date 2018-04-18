/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/position.h"

#include <roq/logging.h>

namespace examples {
namespace common {

namespace {
const double TOLERANCE = 1.0e-8;
}  // namespace

Position::Position(double position)
    : _manual(position),
      _use_position_update(std::isnan(position)),
      _start_of_day(_use_position_update ? 0.0 : _manual) {
  LOG_IF(FATAL, _manual < 0.0) << "Unexpected";
}

void Position::reset() {
  _last_order_id = 0;
  _start_of_day = _use_position_update ? 0.0 : _manual;
  _closed = 0.0;
  _opened = 0.0;
}

bool Position::can_close() const {
  return (_start_of_day - _closed) > -TOLERANCE;
}

double Position::get(PositionType type) const {
  switch (type) {
    case PositionType::StartOfDay: return _start_of_day;
    case PositionType::Closed: return _closed;
    case PositionType::Opened: return _opened;
    case PositionType::Current: return (_start_of_day - _closed) + _opened;
  }
}

void Position::on(const roq::PositionUpdate& position_update) {
  // check if reset() is being used as intended
  LOG_IF(FATAL, _start_of_day != 0.0) << "Unexpected";
  // positions are reported separately for long and short (*not* net)
  LOG_IF(FATAL, position_update.position < -0.0) << "Unexpected";
  if (_use_position_update == false)
    return;
  _last_order_id = position_update.last_order_id;
  // _start_of_day = position_update.yesterday;
  _start_of_day = position_update.position - position_update.yesterday;
}

void Position::open(uint32_t order_id, double quantity) {
  LOG_IF(FATAL, quantity < -0.0) << "Unexpected";
  // coordinate with the position feed
  if (order_id <= _last_order_id)
    return;
  _opened += quantity;
}

void Position::close(uint32_t order_id, double quantity) {
  LOG_IF(FATAL, quantity < -0.0) << "Unexpected";
  // coordinate with the position feed
  if (order_id <= _last_order_id)
    return;
  _closed += quantity;
  if (_start_of_day < (_closed + TOLERANCE))
    LOG(WARNING) << "Today's \"close\" trades exceeds yesterday's \"open\"";
}

std::ostream& Position::write(std::ostream& stream) const {
  return stream << "{"
    "start_of_day=" << get(PositionType::StartOfDay) << ", "
    "closed=" << get(PositionType::Closed) << ", "
    "opened=" << get(PositionType::Opened) << ", "
    "current=" << get(PositionType::Current) << ", "
    "last_order_id=" << _last_order_id <<
    "}";
}

}  // namespace common
}  // namespace examples
