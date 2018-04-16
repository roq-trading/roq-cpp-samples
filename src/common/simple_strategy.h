/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>

#include "common/market_data.h"
#include "common/position.h"

namespace examples {
namespace common {

/**
 * A simple base strategy supporting
 *
 *   # IOC orders
 *   # single exchange
 *   # single symbol
 *   # single gateway
 *   # no exposure / risk management
 *   # aggregated view of MarketByPrice and TradeSummary
 *   # maintenance of yesterday's long/short position
 *   # maintenance of current long/short position
 *
 * A derived strategy should only implement the update() method.
 * Everything else is managed by this base implementation.
 */
class SimpleStrategy : public roq::Strategy {
 public:
  explicit SimpleStrategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& gateway,
      const std::string& account,
      const std::string& exchange,
      const std::string& symbol,
      double tick_size);

  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_symbol() const { return _symbol; }

  const std::string& get_order_template(bool close) const {
    return close ? _close : _open;
  }

  double get_tick_size() const { return _tick_size; }

  double get_long_position(PositionType type) const {
    return _long_position.get(type);
  }
  double get_short_position(PositionType type) const {
    return _short_position.get(type);
  }
  double get_net_position(PositionType type) const {
    return get_long_position(type) - get_short_position(type);
  }

  bool is_ready() const;

  virtual void update(const MarketData&) = 0;

 private:
  // api methods
  const roq::Strategy::subscriptions_t& get_subscriptions() const override {
    return _subscriptions;
  }
  // api event handlers:
  // - timer
  void on(const roq::TimerEvent&) override {}
  // - connection
  void on(const roq::ConnectionStatusEvent&) override {}
  // - batch
  void on(const roq::BatchBeginEvent&) override;
  void on(const roq::BatchEndEvent&) override;
  // - download
  void on(const roq::DownloadBeginEvent&) override;
  void on(const roq::DownloadEndEvent&) override;
  // - order manager or market data
  void on(const roq::GatewayStatusEvent&) override;
  // - order manager update
  void on(const roq::ReferenceDataEvent&) override;
  void on(const roq::MarketStatusEvent&) override;
  void on(const roq::PositionUpdateEvent&) override;
  void on(const roq::OrderUpdateEvent&) override;
  void on(const roq::TradeUpdateEvent&) override {}
  // - order manager response
  void on(const roq::CreateOrderAckEvent&) override;
  void on(const roq::ModifyOrderAckEvent&) override;
  void on(const roq::CancelOrderAckEvent&) override;
  // - market data update
  void on(const roq::MarketByPriceEvent&) override;
  void on(const roq::TradeSummaryEvent&) override;

 protected:
  // create order
  uint32_t create_order(
      roq::Side side,
      double quantity,
      double price,
      const std::string& order_template);

 private:
  // general utilities
  bool filter(const char *exchange, const char *symbol);
  bool parse_open_close(const char *order_template);

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  // configuration
  const std::string _gateway;
  const std::string _open;
  const std::string _close;
  const std::string _account;
  const std::string _exchange;
  const std::string _symbol;
  // subscriptions
  const roq::Strategy::subscriptions_t _subscriptions;
  // state management
  bool _download = false;
  bool _order_manager_ready = false;
  double _tick_size;
  uint32_t _max_order_id = 0;
  bool _market_open = false;
  Position _long_position;
  Position _short_position;
  std::unordered_map<uint32_t, double> _order_traded_quantity;
  MarketData _market_data = {
    .index = 0,
    .exchange = _exchange.c_str(),
    .symbol = _symbol.c_str(),
  };
  bool _market_data_dirty = false;
};

}  // namespace common
}  // namespace examples
