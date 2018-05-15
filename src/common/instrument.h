/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <map>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "common/market_data.h"
#include "common/position.h"

namespace examples {
namespace common {

class Instrument final {
 public:
  explicit Instrument(
      size_t index,
      const std::string& exchange,
      const std::string& symbol,
      double risk_limit,
      double tick_size,
      double multiplier,
      std::vector<Position *>&& positions);

  void reset();  // TODO(thraneh): hide from user

  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }
  double get_risk_limit() const { return _risk_limit; }
  bool can_trade() const { return _tradeable; }
  bool is_ready() const;
  double get_tick_size() const { return _market_data.tick_size; }
  double get_multiplier() const { return _market_data.multiplier; }
  double get_position() const;
  const MarketData& get_market_data() const { return _market_data; }

  void on(const roq::ReferenceDataEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketStatusEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::MarketByPriceEvent& event);  // TODO(thraneh): hide from user
  void on(const roq::TradeSummaryEvent& event);  // TODO(thraneh): hide from user

  void buy_ioc(double quantity, double price);
  void sell_ioc(double quantity, double price);

  std::ostream& write(std::ostream& stream) const;

 private:
  void create_ioc(roq::Side side, double quantity, double price);

 private:
  const size_t _index;
  const std::string _exchange;
  const std::string _symbol;
  const double _risk_limit;
  const bool _tradeable;
  MarketData _market_data;  // aggregator for MarketByPrice and TradeSummary
  bool _market_open = false;

  std::vector<Position *> _positions;


  std::unordered_set<uint32_t> _live_orders;  // HANS -- wrong, per OM
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Instrument& instrument) {
  return instrument.write(stream);
}

}  // namespace common
}  // namespace examples
