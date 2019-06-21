/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/common/position.h"

#include <algorithm>

#include "roq/logging.h"
#include "roq/stream.h"

namespace roq {
namespace samples {
namespace common {

namespace {
constexpr double TOLERANCE = 1.0e-8;
}  // namespace

Position::Position(
    Account& account,
    double long_limit,
    double short_limit,
    double /*long_start_of_day*/,
    double /*short_start_of_day*/)
    : _account(account),
      _long_limit(long_limit),
      _short_limit(short_limit) {
  // TODO(thraneh): long+short start of day
}

PositionEffect Position::get_effect(
    Side side,
    double quantity) const {
  switch (side) {
    case Side::BUY: {
      if ((quantity + _short.closed - _short.start_of_day) < TOLERANCE)
        return PositionEffect::CLOSE;
      if ((_long.opened + quantity - _long_limit) > TOLERANCE)
        return PositionEffect::UNDEFINED;
      break;
    }
    case Side::SELL: {
      if ((quantity + _long.closed - _long.start_of_day) < TOLERANCE)
        return PositionEffect::CLOSE;
      if ((_short.opened + quantity - _short_limit) > TOLERANCE)
        return PositionEffect::UNDEFINED;
      break;
    }
    default: {
      LOG(FATAL) << "Unexpected";  // user error
    }
  }
  return PositionEffect::OPEN;
}

void Position::reset() {
  std::memset(&_long, 0, sizeof(_long));
  std::memset(&_short, 0, sizeof(_short));
}

void Position::on(
    const PositionUpdate& position_update,
    bool download) {
  // (for now) we only refresh on download
  if (!download)
    return;
  // expecting positions to be reported by side (*not* as net position)
  if (position_update.position < -0.0) {
    LOG(WARNING) << "Received negative position for "
      "position_update=" << position_update;
    return;
  }
  // positions are reported separately for long and short
  switch (position_update.side) {
    case Side::BUY: {
      // check if reset() is being used as intended
      LOG_IF(ERROR, _long.start_of_day != 0.0) <<
        "Unexpected -- reset() not used correctly";
      _long.last_trade_id = position_update.last_trade_id;
      _long.max_trade_id = _long.last_trade_id;
      _long.start_of_day = position_update.position_yesterday;
      _long.opened = position_update.position -
        position_update.position_yesterday;
      break;
    }
    case Side::SELL: {
      // check if reset() is being used as intended
      LOG_IF(ERROR, _short.start_of_day != 0.0) <<
        "Unexpected -- reset() not used correctly";
      _short.last_trade_id = position_update.last_trade_id;
      _short.max_trade_id = _short.last_trade_id;  // inherit
      _short.start_of_day = position_update.position_yesterday;
      _short.opened = position_update.position -
        position_update.position_yesterday;
      break;
    }
    default: {
      LOG(WARNING) << "Received unknown or undefined side for "
        "position_update=" << position_update;
    }
  }
}

void Position::on(
    const TradeUpdate& trade_update,
    bool download) {
  auto trade_id = trade_update.trade_id;
  bool close = trade_update.position_effect == PositionEffect::CLOSE;
  // note!
  // trade id's are allowed to arrive randomly during the download phase
  // and we expect trade id's to arrive sequentially during live trading
  // this is because the gateway will guarantee non-duplicates during
  // the download phase (due to caching by trade id)
  // live trading is different -- *all* updates will be published
  switch (trade_update.side) {
    case Side::BUY: {
      if (close) {
        if ((download ? _short.last_trade_id : _short.max_trade_id) < trade_id) {
          _short.closed += trade_update.quantity;
          _short.max_trade_id = std::max(_short.max_trade_id, trade_id);
        }
      } else {
        if ((download ? _long.last_trade_id : _long.max_trade_id) < trade_id) {
          _long.opened += trade_update.quantity;
          _long.max_trade_id = std::max(_long.max_trade_id, trade_id);
        }
      }
      break;
    }
    case Side::SELL: {
      if (close) {
        if ((download ? _long.last_trade_id : _long.max_trade_id) < trade_id) {
          _long.closed += trade_update.quantity;
          _long.max_trade_id = std::max(_long.max_trade_id, trade_id);
        }
      } else {
        if ((download ? _short.last_trade_id : _short.max_trade_id) < trade_id) {
          _short.opened += trade_update.quantity;
          _short.max_trade_id = std::max(_short.max_trade_id, trade_id);
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
    "long_start_of_day=" << _long.start_of_day << ", "
    "long_closed=" << _long.closed << ", "
    "long_opened=" << _long.opened << ", "
    "long_last_trade_id=" << _long.last_trade_id << ", "
    "long_max_trade_id=" << _long.max_trade_id << ", "
    "short_limit=" << _short_limit << ", "
    "short_start_of_day=" << _short.start_of_day << ", "
    "short_closed=" << _short.closed << ", "
    "short_opened=" << _short.opened << ", "
    "short_last_trade_id=" << _short.last_trade_id << ", "
    "short_max_trade_id=" << _short.max_trade_id <<
    "}";
}

}  // namespace common
}  // namespace samples
}  // namespace roq
