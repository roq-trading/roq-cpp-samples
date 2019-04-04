/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <vector>

#include "roq/client.h"

namespace roq {
namespace samples {
namespace collector {

class Collector final : public roq::client::Handler {
 public:
  explicit Collector(roq::client::Dispatcher& dispatcher);

 protected:
  void on(const roq::StartEvent&) override {}
  void on(const roq::StopEvent&) override {}
  void on(const roq::TimerEvent&) override {}
  void on(const roq::ConnectionStatusEvent&) override {}
  void on(const roq::BatchBeginEvent&) override {}
  void on(const roq::BatchEndEvent& event) override {}
  void on(const roq::MarketDataStatusEvent& event) override {}
  void on(const roq::OrderManagerStatusEvent& event) override {}
  void on(const roq::DownloadBeginEvent& event) override {}
  void on(const roq::DownloadEndEvent& event) override {}
  void on(const roq::ReferenceDataEvent& event) override {}
  void on(const roq::MarketStatusEvent& event) override {}
  void on(const roq::SessionStatisticsEvent& event) override {}
  void on(const roq::DailyStatisticsEvent& event) override {}
  void on(const roq::MarketByPriceEvent& event) override;
  void on(const roq::TradeSummaryEvent& event) override {}
  void on(const roq::PositionUpdateEvent& event) override {}
  void on(const roq::OrderUpdateEvent& event) override {}
  void on(const roq::TradeUpdateEvent& event) override {}
  void on(const roq::CreateOrderAckEvent& event) override {}
  void on(const roq::ModifyOrderAckEvent& event) override {}
  void on(const roq::CancelOrderAckEvent& event) override {}
  void on(const roq::CustomMessageEvent& event) override {}

  void write(Metrics& metrics) const override {}

 private:
  roq::client::Dispatcher& _dispatcher;
  uint32_t _order_id = 0;
};

}  // namespace collector
}  // namespace samples
}  // namespace roq
