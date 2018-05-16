/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

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
      std::vector<std::shared_ptr<Position> >&& positions);

  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }
  double get_risk_limit() const { return _risk_limit; }
  bool can_trade() const { return _tradeable; }
  bool is_ready() const;
  double get_tick_size() const { return _market_data.tick_size; }
  double get_multiplier() const { return _market_data.multiplier; }
  double get_position() const;
  const MarketData& get_market_data() const { return _market_data; }

  void on(const roq::MarketDataStatus&);
  void on(const roq::MarketByPrice&);
  void on(const roq::TradeSummary&);

  void on(const roq::ReferenceData&);
  void on(const roq::MarketStatus&);

  void buy_ioc(double quantity, double price);
  void sell_ioc(double quantity, double price);

  std::ostream& write(std::ostream& stream) const;

 private:
  void create_ioc(roq::Side side, double quantity, double price);

 private:
  Instrument(const Instrument&) = delete;
  void operator=(const Instrument&) = delete;

 private:
  const size_t _index;
  const std::string _exchange;
  const std::string _symbol;
  const double _risk_limit;
  const bool _tradeable;
  MarketData _market_data;  // aggregator for MarketByPrice and TradeSummary
  bool _market_open = false;
  bool _market_data_ready = false;
  std::vector<std::shared_ptr<Position> > _positions;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const Instrument& instrument) {
  return instrument.write(stream);
}

}  // namespace common
}  // namespace examples
