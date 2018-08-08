/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>
#include <unordered_map>

namespace examples {
namespace common {

class Account;

class Position final {
 public:
  explicit Position(
      Account& account,
      const std::string& exchange,
      const std::string& symbol,
      bool use_position_update,  // FIXME(thraneh): drop
      double long_limit,
      double short_limit,
      double long_start_of_day,
      double short_start_of_day);

  Account& get_account() { return _account; }
  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }
  void reset();
  double get_net() const;
  roq::PositionEffect get_effect(roq::Side side, double quantity) const;
  void on(const roq::PositionUpdate& position_update, bool download);
  void on(const roq::TradeUpdate& trade_update, bool download);
  std::ostream& write(std::ostream& stream) const;

 private:
  Position(const Position&) = delete;
  void operator=(const Position&) = delete;

 private:
  Account& _account;
  const std::string _exchange;
  const std::string _symbol;
  // long
  const double _long_limit;
  double _long_start_of_day;
  double _long_closed = 0.0;
  double _long_opened = 0.0;
  uint32_t _long_last_trade_id = 0;
  uint32_t _long_max_trade_id = 0;
  // short
  const double _short_limit;
  double _short_start_of_day;
  double _short_closed = 0.0;
  double _short_opened = 0.0;
  uint32_t _short_last_trade_id = 0;
  uint32_t _short_max_trade_id = 0;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Position& position) {
  return position.write(stream);
}

}  // namespace common
}  // namespace examples
