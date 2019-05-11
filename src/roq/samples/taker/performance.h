/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

namespace roq {
namespace samples {
namespace taker {

class Performance final : public client::Collector {
 public:
  Performance() {
  }
  explicit Performance(std::chrono::nanoseconds period) {
  }

 protected:
  // void on(const ConnectionStatusEvent& event) override;
  // void on(const BatchBeginEvent& event) override;
  // void on(const BatchEndEvent& event) override;
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
  void on(const CreateOrderEvent& event) override;
  void on(const ModifyOrderEvent& event) override;
  void on(const CancelOrderEvent& event) override;
  void on(const CreateOrderAckEvent& event) override;
  void on(const ModifyOrderAckEvent& event) override;
  void on(const CancelOrderAckEvent& event) override;

  // void write(Metrics& metrics) const override;

  bool column_type(const std::string& type) const override;
  void extract(
      const std::string& type,
      const std::string& symbol,
      std::vector<std::chrono::nanoseconds>& index,
      std::vector<std::string>& columns,
      std::vector<double>& data) const override;
  void extract(
      const std::string& type,
      const std::string& symbol,
      std::vector<std::chrono::nanoseconds>& index,
      std::vector<double>& columns,
      std::vector<double>& data) const override;
};

}  // namespace taker
}  // namespace samples
}  // namespace roq
