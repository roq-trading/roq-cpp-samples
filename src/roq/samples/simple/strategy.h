/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "roq/client.h"

namespace roq {
namespace samples {
namespace simple {

// The lowest level interface

class Strategy final : public client::Handler {
 public:
  Strategy(
      client::Dispatcher& dispatcher,
      const std::string& exchange,
      const std::vector<std::string_view>& accounts,
      const std::vector<std::string_view>& symbols,
      bool create_orders,
      std::chrono::milliseconds order_timeout);

 protected:
  void on(const StartEvent&) override;
  void on(const StopEvent&) override;
  void on(const TimerEvent&) override;
  void on(const ConnectionStatusEvent&) override;
  void on(const BatchBeginEvent&) override;
  void on(const BatchEndEvent& event) override;
  void on(const MarketDataStatusEvent& event) override;
  void on(const OrderManagerStatusEvent& event) override;
  void on(const DownloadBeginEvent& event) override;
  void on(const DownloadEndEvent& event) override;
  void on(const ReferenceDataEvent& event) override;
  void on(const MarketStatusEvent& event) override;
  void on(const SessionStatisticsEvent& event) override;
  void on(const DailyStatisticsEvent& event) override;
  void on(const MarketByPriceEvent& event) override;
  void on(const TradeSummaryEvent& event) override;
  void on(const PositionUpdateEvent& event) override;
  void on(const OrderUpdateEvent& event) override;
  void on(const TradeUpdateEvent& event) override;
  void on(const CreateOrderAckEvent& event) override;
  void on(const ModifyOrderAckEvent& event) override;
  void on(const CancelOrderAckEvent& event) override;
  void on(const CustomMessageEvent& event) override;

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
