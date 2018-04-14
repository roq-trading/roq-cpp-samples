/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <roq/api.h>

#include <chrono>  // NOLINT
#include <string>
#include <utility>

#include "reference/config.h"
#include "reference/order_manager.h"
#include "reference/position_manager.h"
#include "reference/risk_manager.h"
#include "reference/trading_model.h"

namespace examples {
namespace reference {

class GatewayManager final : public roq::Strategy {
 public:
  // constructor
  GatewayManager(roq::Strategy::Dispatcher& dispatcher,
                 Config&& config);

 protected:
  // methods
  const roq::Strategy::subscriptions_t& get_subscriptions() const override;
  // event handlers
  void on(const roq::TimerEvent&) override;
  void on(const roq::ConnectionStatusEvent&) override;
  void on(const roq::BatchBeginEvent&) override;
  void on(const roq::BatchEndEvent&) override;
  void on(const roq::DownloadBeginEvent&) override;
  void on(const roq::DownloadEndEvent&) override;
  void on(const roq::GatewayStatusEvent&) override;
  void on(const roq::ReferenceDataEvent&) override;
  void on(const roq::MarketStatusEvent&) override;
  void on(const roq::MarketByPriceEvent&) override;
  void on(const roq::TradeSummaryEvent&) override;
  void on(const roq::CreateOrderAckEvent&) override;
  void on(const roq::ModifyOrderAckEvent&) override;
  void on(const roq::CancelOrderAckEvent&) override;
  void on(const roq::OrderUpdateEvent&) override;
  void on(const roq::TradeUpdateEvent&) override;
  void on(const roq::PositionUpdateEvent&) override;

 private:
  // state management
  void check(const roq::MessageInfo&);

 private:
  // disallow default behaviour
  GatewayManager() = delete;
  GatewayManager(GatewayManager&) = delete;
  GatewayManager& operator=(GatewayManager&) = delete;

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  Config _config;
  PositionManager _position_manager;
  RiskManager _risk_manager;
  OrderManager _order_manager;
  TradingModel _trading_model;
  // state management
  bool _order_manager_ready = false;
  bool _market_data_ready = false;
  bool _market_open = false;
  // consistency check
  typedef std::chrono::system_clock::time_point time_point_t;
  time_point_t _last_update_time;
};

}  // namespace reference
}  // namespace examples
