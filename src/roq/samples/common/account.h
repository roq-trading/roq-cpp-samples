/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "roq/api.h"

#include "roq/client.h"

#include "roq/samples/common/config.h"
#include "roq/samples/common/position.h"

namespace roq {
namespace samples {
namespace common {

class Account final : NonCopyable {
 public:
  Account(
      client::Dispatcher& dispatcher,
      const std::string_view& name,
      const Config& config);

  inline const std::string& get_name() const {
    return _name;
  }
  inline bool is_downloading() const {
    return _download;
  }
  inline bool is_ready() const {
    return _order_manager_ready;
  }

  Position& get_position(
      const std::string& /*exchange*/,
      const std::string& symbol) {
    return _positions.at(symbol);
  }

  void on(const TimerEvent&);

  void on(const DownloadBegin&);
  void on(const DownloadEnd&);
  void on(const PositionUpdate&);
  void on(const OrderUpdate&);
  void on(const TradeUpdate&);
  void on(const OrderManagerStatus&);

  uint32_t create_order(
      const std::string_view& exchange,
      const std::string_view& symbol,
      Side side,
      double quantity,
      double price,
      TimeInForce time_in_force,
      PositionEffect position_effect,
      const std::string_view& order_template = "default");

  void modify_order(
      uint32_t order_id,
      double quantity_change,
      double limit_price);

  void cancel_order(uint32_t order_id);

  void on(const CreateOrderAck&);
  void on(const ModifyOrderAck&);
  void on(const CancelOrderAck&);

  std::ostream& write(std::ostream& stream) const;

 protected:
  void update_max_order_id(uint32_t order_id);
  inline bool is_order_live(uint32_t order_id) const {
    return _live_orders.find(order_id) != _live_orders.end();
  }

 private:
  client::Dispatcher& _dispatcher;
  std::string _name;
  bool _download = false;
  uint32_t _max_order_id = 0;
  bool _order_manager_ready = false;
  std::unordered_set<uint32_t> _live_orders;
  std::unordered_map<std::string, Position> _positions;
};

}  // namespace common
}  // namespace samples
}  // namespace roq

inline std::ostream& operator<<(
    std::ostream& stream,
    const roq::samples::common::Account& account) {
  return account.write(stream);
}
