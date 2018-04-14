/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>

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
      const std::string& instrument,
      double risk_limit,
      double long_position,
      double short_position,
      double tick_size);
  void reset();
  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_instrument() const { return _instrument; }
  double get_risk_limit() const { return _risk_limit; }
  bool can_trade() const { return _tradeable; }
  bool is_ready() const { return _market_open; }
  double get_long_position(PositionType type) const;
  double get_short_position(PositionType type) const;
  double get_net_position(PositionType type) const;
  double get_tick_size() const { return _tick_size; }
  const MarketData& get_market_data() const { return _market_data; }
  void on(const roq::ReferenceDataEvent& event);
  void on(const roq::MarketStatusEvent& event);
  void on(const roq::PositionUpdateEvent& event);
  void on(const roq::OrderUpdateEvent& event);
  void on(const roq::CreateOrderAckEvent& event);
  void on(const roq::ModifyOrderAckEvent& event);
  void on(const roq::CancelOrderAckEvent& event);
  void on(const roq::MarketByPriceEvent& event);
  void on(const roq::TradeSummaryEvent& event);
  uint32_t create_order(
      roq::TradeDirection direction,
      double quantity,
      double price,
      const std::string& order_template);
  std::ostream& write(std::ostream& stream) const;

 private:
  const size_t _index;
  Gateway& _gateway;
  const std::string _exchange;
  const std::string _instrument;
  double _risk_limit;
  bool _tradeable;
  bool _market_open = false;
  Position _long_position;
  Position _short_position;
  double _tick_size;
  MarketData _market_data;  // aggregator for MarketByPrice and TradeSummary
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Instrument& instrument) {
  return instrument.write(stream);
}

}  // namespace common
}  // namespace examples
