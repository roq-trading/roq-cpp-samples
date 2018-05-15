/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <unordered_map>

#include "common/position.h"

#include "simple/config.h"

namespace examples {
namespace common {

class Instrument;

class Account final {
 public:
  Account(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& name,
      const Config& config);

  const std::string& get_name() const { return _name; }

  Position& get_position(
      const std::string& exchange,
      const std::string& symbol);

  bool is_ready() const { return _ready; }

  void on(const roq::TimerEvent&);

  void on(const roq::DownloadBegin&);
  void on(const roq::DownloadEnd&);
  void on(const roq::PositionUpdate&);
  void on(const roq::OrderUpdate&);
  void on(const roq::TradeUpdate&);
  void on(const roq::OrderManagerStatus&);

  uint32_t create_order(
      const std::string& exchange,
      const std::string& symbol,
      roq::Side side,
      double quantity,
      double price,
      roq::TimeInForce time_in_force,
      roq::PositionEffect position_effect,
      const std::string& order_template,
      Instrument& instrument);
  void modify_order(
      uint32_t order_id,
      double quantity_change,
      double limit_price);
  void cancel_order(uint32_t order_id);

  void on(const roq::CreateOrderAck&);
  void on(const roq::ModifyOrderAck&);
  void on(const roq::CancelOrderAck&);

  std::ostream& write(std::ostream& stream) const;

 private:
  void update_max_order_id(uint32_t order_id);
  bool apply(
      const std::string& exchange,
      const std::string& symbol,
      std::function<void(Position&)> function);
  bool is_order_live(uint32_t order_id) const;

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  std::string _name;
  bool _download = false;
  uint32_t _max_order_id = 0;
  bool _ready = false;
  std::vector<Position> _positions;
  std::unordered_map<std::string, Position *> _positions_by_symbol;

  // HANS -- why the Instrument callback?
  // ---> should rather monitor for timeout
  std::unordered_map<uint32_t, Instrument *> _live_orders;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Account& account) {
  return account.write(stream);
}

}  // namespace common
}  // namespace examples
