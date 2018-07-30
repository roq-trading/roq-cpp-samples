/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/position.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

namespace examples {
namespace common {

namespace {
const double TOLERANCE = 1.0e-8;
}  // namespace

Position::Position(
    Account& account,
    const std::string& exchange,
    const std::string& symbol,
    bool use_position_update,
    double long_limit,
    double short_limit,
    double long_start_of_day,
    double short_start_of_day)
    : _account(account),
      _exchange(exchange),
      _symbol(symbol),
      _use_position_update(use_position_update),
      _long_limit(long_limit),
      _long_start_of_day(long_start_of_day),
      _short_limit(short_limit),
      _short_start_of_day(short_start_of_day) {
  LOG_IF(FATAL, _long_start_of_day < 0.0) << "Unexpected";
  LOG_IF(FATAL, _short_start_of_day < 0.0) << "Unexpected";
}

void Position::reset() {
  if (_use_position_update) {
    // long
    _long_last_order_local_id = 0;
    _long_start_of_day = 0.0;
    _long_last_trade_id = 0;
    // short
    _short_last_order_local_id = 0;
    _short_start_of_day = 0.0;
    _short_last_trade_id = 0;
  }
  // long
  _long_closed = 0.0;
  _long_opened = 0.0;
  // short
  _short_closed = 0.0;
  _short_opened = 0.0;
  // ...
  _traded_quantity.clear();
}

double Position::get_net() const {
  auto long_position = _long_start_of_day - _long_closed + _long_opened;
  auto short_position = _short_start_of_day - _short_closed + _short_opened;
  return long_position - short_position;
}

roq::PositionEffect Position::get_effect(
    roq::Side side,
    double quantity) const {
  switch (side) {
    case roq::Side::Buy: {
      if ((quantity + _short_closed - _short_start_of_day) < TOLERANCE)
        return roq::PositionEffect::Close;
      if ((_long_opened + quantity - _long_limit) > TOLERANCE)
        return roq::PositionEffect::Undefined;
      break;
    }
    case roq::Side::Sell: {
      if ((quantity + _long_closed - _long_start_of_day) < TOLERANCE)
        return roq::PositionEffect::Close;
      if ((_short_opened + quantity - _short_limit) > TOLERANCE)
        return roq::PositionEffect::Undefined;
      break;
    }
    default:
      LOG(FATAL) << "Unexpected";  // user error
  }
  return roq::PositionEffect::Open;
}

void Position::on(const roq::PositionUpdate& position_update) {
  // expecting positions to be reported by side (not net position)
  if (position_update.position < -0.0) {
    LOG(WARNING) << "Received negative position for "
      "position_update=" << position_update;
    return;
  }
  // drop?
  if (!_use_position_update) {
    VLOG(2) << "Dropping position update. (Not required).";
    return;
  }
  // positions are reported separately for long and short
  switch (position_update.side) {
    case roq::Side::Buy: {
      // check if reset() is being used as intended
      if (_long_start_of_day != 0.0)
        LOG(ERROR) << "Unexpected -- reset() not used correctly";
      _long_last_order_local_id = position_update.last_order_local_id;
      _long_last_trade_id = position_update.last_trade_id;
      _long_start_of_day = position_update.yesterday;
      _long_opened = position_update.position - position_update.yesterday;
      break;
    }
    case roq::Side::Sell: {
      // check if reset() is being used as intended
      if (_short_start_of_day != 0.0)
        LOG(ERROR) << "Unexpected -- reset() not used correctly";
      _short_last_order_local_id = position_update.last_order_local_id;
      _short_last_trade_id = position_update.last_trade_id;
      _short_start_of_day = position_update.yesterday;
      _short_opened = position_update.position - position_update.yesterday;
      break;
    }
    default: {
      LOG(WARNING) << "Received unknown or undefined side for "
        "position_update=" << position_update;
    }
  }
}

void Position::on(const roq::TradeUpdate& trade_update) {
  // FIXME(thraneh): reconcile trade updates with order updates
}

void Position::on(const roq::OrderUpdate& order_update) {
  auto order_id = order_update.order_id;
  auto& previous = _traded_quantity[order_id];
  auto fill_quantity = std::max(0.0, order_update.traded_quantity - previous);
  previous = order_update.traded_quantity;
  if (fill_quantity < TOLERANCE) {
    VLOG(2) << "No fill quantity, dropping order update";
    return;
  }
  bool close = order_update.position_effect == roq::PositionEffect::Close;
  auto order_local_id = order_update.order_local_id;
  switch (order_update.side) {
    case roq::Side::Buy: {
      if (close) {
        if (_short_last_order_local_id < order_local_id) {
          _short_closed += fill_quantity;
        } else {
          VLOG(2) << "Repeat order update, "
            "short_last_order_local_id=" << _short_last_order_local_id << ", "
            "order_local_id=" << order_local_id;
        }
      } else {
        if (_long_last_order_local_id < order_local_id) {
          _long_opened += fill_quantity;
        } else {
          VLOG(2) << "Repeat order update, "
            "long_last_order_local_id=" << _long_last_order_local_id << ", "
            "order_local_id=" << order_local_id;
        }
      }
      break;
    }
    case roq::Side::Sell: {
      if (close) {
        if (_long_last_order_local_id < order_local_id) {
          _long_closed += fill_quantity;
        } else {
          VLOG(2) << "Repeat order update, "
            "long_last_order_local_id=" << _long_last_order_local_id << ", "
            "order_local_id=" << order_local_id;
        }
      } else {
        if (_short_last_order_local_id < order_local_id) {
          _short_opened += fill_quantity;
        } else {
          VLOG(2) << "Repeat order update, "
            "short_last_order_local_id=" << _short_last_order_local_id << ", "
            "order_local_id=" << order_local_id;
        }
      }
      break;
    }
    default: {
      LOG(WARNING) << "Received unknown or undefined side for "
        "order_update=" << order_update;
    }
  }
}

std::ostream& Position::write(std::ostream& stream) const {
  return stream << "{"
    "use_position_update=" << (_use_position_update ? "true" : "false") << ", "
    "long_limit=" << _long_limit << ", "
    "long_last_order_local_id=" << _long_last_order_local_id << ", "
    "long_start_of_day=" << _long_start_of_day << ", "
    "long_closed=" << _long_closed << ", "
    "long_opened=" << _long_opened << ", "
    "long_last_trade_id=" << _long_last_trade_id << ", "
    "short_limit=" << _short_limit << ", "
    "short_last_order_local_id=" << _short_last_order_local_id << ", "
    "short_start_of_day=" << _short_start_of_day << ", "
    "short_closed=" << _short_closed << ", "
    "short_opened=" << _short_opened << ", "
    "short_last_trade_id=" << _short_last_trade_id <<
    "}";
}

}  // namespace common
}  // namespace examples
