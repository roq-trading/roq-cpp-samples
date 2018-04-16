/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>
#include <unordered_set>

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
      double risk_limit,
      double long_position,
      double short_position,
      double tick_size);
  void reset();  // TODO(thraneh): hide from user
  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }
  double get_risk_limit() const { return _risk_limit; }
  bool can_trade() const { return _tradeable; }
  bool is_ready() const;
  double get_long_position(PositionType type) const;
  double get_short_position(PositionType type) const;
  double get_net_position(PositionType type) const;
  double get_tick_size() const { return _tick_size; }
  const MarketData& get_market_data() const { return _market_data; }
  void on(const roq::ReferenceDataEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketStatusEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::PositionUpdateEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::OrderUpdateEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketByPriceEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::TradeSummaryEvent& event);  // TODO(thraneh): hide from user
  uint32_t create_order(
      roq::TradeDirection direction,
      double quantity,
      double price,
      const std::string& order_template);
  void modify_order(
      uint32_t order_id,
      double quantity_change,
      double limit_price);
  void cancel_order(uint32_t order_id);
  void on(const roq::CreateOrderAckEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::ModifyOrderAckEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::CancelOrderAckEvent& event);  // TODO(thraneh): hide from user
  bool is_order_live(uint32_t order_id) const;
  std::ostream& write(std::ostream& stream) const;

 private:
  const size_t _index;
  Gateway& _gateway;
  const std::string _exchange;
  const std::string _symbol;
  double _risk_limit;
  bool _tradeable;
  bool _market_open = false;
  Position _long_position;
  Position _short_position;
  double _tick_size;
  MarketData _market_data;  // aggregator for MarketByPrice and TradeSummary
  std::unordered_set<uint32_t> _live_orders;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Instrument& instrument) {
  return instrument.write(stream);
}

}  // namespace common
}  // namespace examples
