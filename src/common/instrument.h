/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <map>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "common/gateway.h"
#include "common/market_data.h"
#include "common/position.h"

namespace examples {
namespace common {

class Instrument final {
 public:
  explicit Instrument(
      size_t index,
      Gateway& gateway,
      const std::string& exchange,
      const std::string& symbol,
      const std::map<std::string, std::pair<double, double> >& accounts,
      double risk_limit,
      double tick_size);
  void reset();  // TODO(thraneh): hide from user
  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }
  double get_risk_limit() const { return _risk_limit; }
  bool can_trade() const { return _tradeable; }
  bool is_ready() const;
  double get_tick_size() const { return _tick_size; }
  double get_position() const;
  const MarketData& get_market_data() const { return _market_data; }
  void on(const roq::ReferenceDataEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketStatusEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::PositionUpdateEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::OrderUpdateEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::TradeUpdateEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketByPriceEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::TradeSummaryEvent& event);  // TODO(thraneh): hide from user
  // order management
  uint32_t create_order(
      const std::string& account,
      roq::Side side,
      double quantity,
      double price,
      roq::TimeInForce time_in_force,
      roq::PositionEffect position_effect,
      const std::string& order_template);
  void modify_order(
      const std::string& account,
      uint32_t order_id,
      double quantity_change,
      double limit_price);
  void cancel_order(
      const std::string& account,
      uint32_t order_id);
  // - helper
  void buy_ioc(double quantity, double price);
  void sell_ioc(double quantity, double price);
  void on(const roq::CreateOrderAckEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::ModifyOrderAckEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::CancelOrderAckEvent& event);  // TODO(thraneh): hide from user
  bool is_order_live(uint32_t order_id) const;
  std::ostream& write(std::ostream& stream) const;

 private:
  void create_ioc(roq::Side side, double quantity, double price);

 private:
  const size_t _index;
  Gateway& _gateway;
  const std::string _exchange;
  const std::string _symbol;
  std::unordered_map<std::string, Position> _positions;
  const double _risk_limit;
  const bool _tradeable;
  MarketData _market_data;  // aggregator for MarketByPrice and TradeSummary
  double _tick_size;
  bool _market_open = false;
  std::unordered_set<uint32_t> _live_orders;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Instrument& instrument) {
  return instrument.write(stream);
}

}  // namespace common
}  // namespace examples
