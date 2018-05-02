/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>

namespace examples {
namespace common {

class Instrument;

class Gateway final {
 public:
  Gateway(roq::Strategy::Dispatcher& dispatcher, const std::string& name);
  bool is_downloading() const { return _download; }
  bool is_ready() const { return _download == false && _order_manager_ready; }
  void on(const roq::TimerEvent& event);
  void on(const roq::DownloadBeginEvent& event);
  void on(const roq::DownloadEndEvent& event);
  void on(const roq::GatewayStatusEvent& event);
  void on(const roq::OrderUpdateEvent& event, Instrument *instrument = nullptr);
  uint32_t create_order(
      const std::string& account,
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
      const std::string& account,
      uint32_t order_id,
      double quantity_change,
      double limit_price);
  void cancel_order(
      const std::string& account,
      uint32_t order_id);
  void on(const roq::CreateOrderAckEvent& event);
  void on(const roq::ModifyOrderAckEvent& event);
  void on(const roq::CancelOrderAckEvent& event);

 private:
  void update_max_order_id(uint32_t order_id);
  bool is_order_live(uint32_t order_id) const;

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  const std::string _name;
  bool _download = false;
  bool _order_manager_ready = false;
  uint32_t _max_order_id = 0;
  std::unordered_map<uint32_t, Instrument *> _live_orders;
};

}  // namespace common
}  // namespace examples
