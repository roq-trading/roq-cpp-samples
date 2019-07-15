/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace samples {
namespace common {

class Account;

class Position final : NonCopyable {
 public:
  Position(
      Account& account,
      double long_limit,
      double short_limit,
      double long_start_of_day,
      double short_start_of_day);

  inline Account& get_account() {
    return _account;
  }

  double get_net() const {
    auto long_position = _long.start_of_day - _long.closed + _long.opened;
    auto short_position = _short.start_of_day - _short.closed + _short.opened;
    return long_position - short_position;
  }

  PositionEffect get_effect(Side side, double quantity) const;

  void reset();

  void on(const PositionUpdate& position_update, bool download);
  void on(const TradeUpdate& trade_update, bool download);

  std::ostream& write(std::ostream& stream) const;

 private:
  Account& _account;
  const double _long_limit;
  const double _short_limit;
  struct Directional final {
    double start_of_day = 0.0;
    double closed = 0.0;
    double opened = 0.0;
    uint32_t last_trade_id = 0;
    uint32_t max_trade_id = 0;
  };
  Directional _long;
  Directional _short;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Position& position) {
  return position.write(stream);
}

}  // namespace common
}  // namespace samples
}  // namespace roq
