/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>

namespace examples {
namespace order_management {

class BaseStrategy : public roq::Strategy {
 public:
  explicit BaseStrategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& exchange,
      const std::string& instrument,
      const std::string& gateway,
      const std::string& ioc_open,
      const std::string& ioc_close,
      const std::string& gtc_open,
      const std::string& gtc_close);

  double get_tick_size() const { return _tick_size; }
  double get_long_position() const { return _long_position; }
  double get_short_position() const { return _short_position; }

  bool is_ready() const;

  virtual void update_signal(const roq::MarketByPrice&) = 0;
  virtual void try_trade() = 0;

 private:
  // event handlers:
  // - timer
  void on(const roq::TimerEvent&) override;
  // - connection
  void on(const roq::ConnectionStatusEvent&) override {}
  // - batch
  void on(const roq::BatchBeginEvent&) override {}
  void on(const roq::BatchEndEvent&) override {}
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
  void on(const roq::TradeSummaryEvent&) override {}

 protected:
  // create order helpers
  uint32_t buy_ioc_open(const double quantity, const double price);
  uint32_t sell_ioc_open(const double quantity, const double price);
  uint32_t buy_ioc_close(const double quantity, const double price);
  uint32_t sell_ioc_close(const double quantity, const double price);
  uint32_t buy_gtc_open(const double quantity, const double price);
  uint32_t sell_gtc_open(const double quantity, const double price);
  uint32_t buy_gtc_close(const double quantity, const double price);
  uint32_t sell_gtc_close(const double quantity, const double price);
  uint32_t create_order(
      roq::TradeDirection direciton,
      const double quantity,
      const double price,
      const std::string& order_template);

 private:
  // position management
  void add_trade(roq::TradeDirection direction, double quantity);

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
  const std::string _gtc_open;
  const std::string _gtc_close;
  // state management
  bool _download = false;
  bool _order_manager_ready = false;
  double _tick_size = 0.01;
  uint32_t _max_order_id = 0;
  bool _market_open = false;
  double _long_position = 0.0;
  double _short_position = 0.0;
  std::unordered_map<uint32_t, double> _order_traded_quantity;
};

}  // namespace order_management
}  // namespace examples
