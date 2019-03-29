/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "roq/api.h"
#include "roq/client.h"

#include "roq/samples/common/market_data.h"
#include "roq/samples/common/position.h"

namespace roq {
namespace samples {
namespace common {

class Instrument final : NonCopyable {
 public:
  Instrument(
      size_t index,
      const std::string& exchange,
      const std::string& symbol,
      const double net_limit,
      const double tick_size,
      const double multiplier,
      std::vector<Position *>&& positions);

  inline size_t index() const {
    return _index;
  }
  inline const std::string& get_exchange() const {
    return _exchange;
  }
  inline const std::string& get_symbol() const {
    return _symbol;
  }
  inline bool can_trade() const {
    return _tradeable;
  }
  inline bool is_ready() const {
    return _market_data_ready && _market_open;
  }
  inline double get_tick_size() const {
    return _market_data.tick_size;
  }
  inline double get_multiplier() const {
    return _market_data.multiplier;
  }
  inline const MarketData& get_market_data() const {
    return _market_data;
  }

  double get_position() const;

  void on(const MarketDataStatus&);
  void on(const SessionStatistics&);
  void on(const DailyStatistics&);
  void on(const MarketByPrice&);
  void on(const TradeSummary&);
  void on(const ReferenceData&);
  void on(const MarketStatus&);

  inline void buy_ioc(double quantity, double price) {
    create_ioc(Side::BUY, quantity, price);
  }
  inline void sell_ioc(double quantity, double price) {
    create_ioc(Side::SELL, quantity, price);
  }

 protected:
  void create_ioc(Side side, double quantity, double price);

 private:
  const size_t _index;
  const std::string _exchange;
  const std::string _symbol;
  const double _net_limit;
  MarketData _market_data;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  bool _market_open = false;
  bool _market_data_ready = false;
  const bool _tradeable;
  std::vector<Position *> _positions;
};

}  // namespace common
}  // namespace samples
}  // namespace roq
