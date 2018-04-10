/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>

#include "simple/market_data.h"

namespace examples {
namespace simple {

/**
 * A simple base strategy supporting
 *
 *   # IOC orders
 *   # single exchange
 *   # single instrument
 *   # single gateway
 *   # no exposure / risk management
 *   # aggregated view of MarketByPrice and TradeSummary
 *   # maintenance of yesterday's long/short position
 *   # maintenance of current long/short position
 *
 * A derived strategy should only implement the update() method.
 * Everything else is managed by this base implementation.
 */
class BaseStrategy : public roq::Strategy {
 public:
  explicit BaseStrategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& exchange,
      const std::string& instrument,
      const std::string& gateway);

  const std::string& get_exchange() const { return _exchange; }
  const std::string& get_instrument() const { return _instrument; }

  const std::string& get_order_template(bool close) const {
    return close ? _ioc_close : _ioc_open;
  }

  double get_tick_size() const { return _tick_size; }

  enum class PositionType {
    StartOfDay,   // today's start of day position
    NewActivity,  // today's new activity
    Current       // current position
  };

  double get_long_position(PositionType type) const;
  double get_short_position(PositionType type) const;

  double get_net_position(PositionType type) const;

  bool is_ready() const;

  virtual void update(const MarketData&) = 0;

 private:
  // api event handlers:
  // - timer
  void on(const roq::TimerEvent&) override;
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
      roq::TradeDirection direction,
      const double quantity,
      const double price,
      const std::string& order_template);

 private:
  // general utilities
  bool filter(const char *exchange, const char *instrument);
  bool parse_open_close(const char *order_template);

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  // configuration
  const std::string _exchange;
  const std::string _instrument;
  const std::string _gateway;
  const std::string _ioc_open;
  const std::string _ioc_close;
  // state management
  bool _download = false;
  bool _order_manager_ready = false;
  double _tick_size = 0.2;
  uint32_t _max_order_id = 0;
  bool _market_open = false;
  double _long_position_sod = 0.0;
  double _short_position_sod = 0.0;
  double _long_position_new = 0.0;
  double _short_position_new = 0.0;
  std::unordered_map<uint32_t, double> _order_traded_quantity;
  MarketData _market_data = {};
  bool _market_data_dirty = false;
};

}  // namespace simple
}  // namespace examples
