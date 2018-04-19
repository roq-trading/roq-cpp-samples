/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <unordered_map>

namespace examples {
namespace common {

class Position final {
 public:
  explicit Position(
      bool use_position_update,
      double long_position,
      double short_position);
  void reset();
  double get_net() const;
  roq::PositionEffect get_effect(roq::Side side, double quantity) const;
  void on(const roq::PositionUpdate& position_update);
  void on(const roq::OrderUpdate& order_update);
  void on(const roq::TradeUpdate& trade_update);
  std::ostream& write(std::ostream& stream) const;

 private:
  const bool _use_position_update;
  // long
  uint32_t _long_last_order_id = 0;
  double _long_start_of_day;
  double _long_closed = 0.0;
  double _long_opened = 0.0;
  uint32_t _long_last_trade_id = 0;
  // short
  uint32_t _short_last_order_id = 0;
  double _short_start_of_day;
  double _short_closed = 0.0;
  double _short_opened = 0.0;
  uint32_t _short_last_trade_id = 0;
  // order
  std::unordered_map<uint32_t, double> _traded_quantity;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Position& position) {
  return position.write(stream);
}

}  // namespace common
}  // namespace examples
