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
      _long_limit(long_limit),
      _long_start_of_day(long_start_of_day),
      _short_limit(short_limit),
      _short_start_of_day(short_start_of_day) {
  LOG_IF(FATAL, _long_start_of_day < 0.0) << "Unexpected";
  LOG_IF(FATAL, _short_start_of_day < 0.0) << "Unexpected";
}

void Position::reset() {
  // long
  _long_start_of_day = 0.0;
  _long_closed = 0.0;
  _long_opened = 0.0;
  _long_last_trade_id = 0;
  _long_max_trade_id = 0;
  // short
  _short_start_of_day = 0.0;
  _short_closed = 0.0;
  _short_opened = 0.0;
  _short_last_trade_id = 0;
  _short_max_trade_id = 0;
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
  // positions are reported separately for long and short
  switch (position_update.side) {
    case roq::Side::Buy: {
      // check if reset() is being used as intended
      if (_long_start_of_day != 0.0)
        LOG(ERROR) << "Unexpected -- reset() not used correctly";
      _long_last_trade_id = position_update.last_trade_id;
      _long_max_trade_id = _long_last_trade_id;
      _long_start_of_day = position_update.yesterday;
      _long_opened = position_update.position - position_update.yesterday;
      break;
    }
    case roq::Side::Sell: {
      // check if reset() is being used as intended
      if (_short_start_of_day != 0.0)
        LOG(ERROR) << "Unexpected -- reset() not used correctly";
      _short_last_trade_id = position_update.last_trade_id;
      _short_max_trade_id = _short_last_trade_id;  // inherit
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

void Position::on(const roq::TradeUpdate& trade_update, bool download) {
  auto trade_id = trade_update.trade_id;
  bool close = trade_update.position_effect == roq::PositionEffect::Close;
  // note!
  // trades may arrive randomly during the download phase and
  // expected to arrive sequentially during live trading
  switch (trade_update.side) {
    case roq::Side::Buy: {
      if (close) {
        if ((download ? _short_last_trade_id : _short_max_trade_id) < trade_id) {
          _short_closed += trade_update.quantity;
          _short_max_trade_id = std::max(_short_max_trade_id, trade_id);
        }
      } else {
        if ((download ? _long_last_trade_id : _long_max_trade_id) < trade_id) {
          _long_opened += trade_update.quantity;
          _long_max_trade_id = std::max(_long_max_trade_id, trade_id);
        }
      }
      break;
    }
    case roq::Side::Sell: {
      if (close) {
        if ((download ? _long_last_trade_id : _long_max_trade_id) < trade_id) {
          _long_closed += trade_update.quantity;
          _long_max_trade_id = std::max(_long_max_trade_id, trade_id);
        }
      } else {
        if ((download ? _short_last_trade_id : _short_max_trade_id) < trade_id) {
          _short_opened += trade_update.quantity;
          _short_max_trade_id = std::max(_short_max_trade_id, trade_id);
        }
      }
      break;
    }
    default: {
      LOG(WARNING) << "Received unknown or undefined side for "
        "trade_update=" << trade_update;
    }
  }
}

std::ostream& Position::write(std::ostream& stream) const {
  return stream << "{"
    "long_limit=" << _long_limit << ", "
    "long_start_of_day=" << _long_start_of_day << ", "
    "long_closed=" << _long_closed << ", "
    "long_opened=" << _long_opened << ", "
    "long_last_trade_id=" << _long_last_trade_id << ", "
    "long_max_trade_id=" << _long_max_trade_id << ", "
    "short_limit=" << _short_limit << ", "
    "short_start_of_day=" << _short_start_of_day << ", "
    "short_closed=" << _short_closed << ", "
    "short_opened=" << _short_opened << ", "
    "short_last_trade_id=" << _short_last_trade_id << ", "
    "short_max_trade_id=" << _short_max_trade_id <<
    "}";
}

}  // namespace common
}  // namespace examples
