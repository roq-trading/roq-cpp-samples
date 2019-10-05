/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "roq/client.h"

#include "roq/samples/simple/config.h"

namespace roq {
namespace samples {
namespace simple {

// The lowest level interface

class Strategy final : public client::Handler {
 public:
  Strategy(
      client::Dispatcher& dispatcher,
      Config& config);

 protected:
  void operator()(const StartEvent&) override;
  void operator()(const StopEvent&) override;
  void operator()(const TimerEvent&) override;
  void operator()(const ConnectionStatusEvent&) override;
  void operator()(const BatchBeginEvent&) override;
  void operator()(const BatchEndEvent& event) override;
  void operator()(const MarketDataStatusEvent& event) override;
  void operator()(const OrderManagerStatusEvent& event) override;
  void operator()(const DownloadBeginEvent& event) override;
  void operator()(const DownloadEndEvent& event) override;
  void operator()(const ReferenceDataEvent& event) override;
  void operator()(const MarketStatusEvent& event) override;
  void operator()(const SessionStatisticsEvent& event) override;
  void operator()(const DailyStatisticsEvent& event) override;
  void operator()(const MarketByPriceEvent& event) override;
  void operator()(const TradeSummaryEvent& event) override;
  void operator()(const PositionUpdateEvent& event) override;
  void operator()(const OrderUpdateEvent& event) override;
  void operator()(const TradeUpdateEvent& event) override;
  void operator()(const CreateOrderAckEvent& event) override;
  void operator()(const ModifyOrderAckEvent& event) override;
  void operator()(const CancelOrderAckEvent& event) override;
  void operator()(const CustomMessageEvent& event) override;

  void write(Metrics& metrics) const override;

  void process_update(std::chrono::nanoseconds now);

  void try_trade(
      Side side,
      double price,
      double quantity,
      std::chrono::nanoseconds now);

 private:
  client::Dispatcher& _dispatcher;
  // configuration:
  const std::string _exchange;
  const std::string _trade_account;
  const std::string _trade_symbol;
  const bool _create_orders;
  const std::chrono::nanoseconds _create_order_timeout;
  // state management:
  // - order manager
  bool _order_manager_ready = false;
  uint32_t _order_id = 0;
  std::chrono::nanoseconds _order_timeout = {};
  // - market data
  bool _market_data_ready = false;
  Layer _depth[5] = {};
  std::unique_ptr<client::DepthBuilder> _market_by_price;
  bool _dirty = false;
  bool _market_open = false;
};

}  // namespace simple
}  // namespace samples
}  // namespace roq
