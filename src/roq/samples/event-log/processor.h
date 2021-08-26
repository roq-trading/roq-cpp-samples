/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/client/event_log_reader.h"

namespace roq {
namespace samples {
namespace event_log {

class Processor final : public client::EventLogReader::Handler {
 public:
  explicit Processor(const std::string_view &path);

  Processor(Processor &&) = default;
  Processor(const Processor &) = delete;

  void dispatch();

 protected:
  // config
  void operator()(const Event<GatewaySettings> &) override;

  // stream
  void operator()(const Event<StreamStatus> &) override;
  void operator()(const Event<ExternalLatency> &) override;
  void operator()(const Event<RateLimitUsage> &) override;

  // service
  void operator()(const Event<GatewayStatus> &) override;

  // market data
  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;
  void operator()(const Event<TopOfBook> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<MarketByOrderUpdate> &) override;
  void operator()(const Event<TradeSummary> &) override;
  void operator()(const Event<StatisticsUpdate> &) override;

  // order actions (from client, note! source == user)
  void operator()(const Event<CreateOrder> &) override;
  void operator()(const Event<ModifyOrder> &) override;
  void operator()(const Event<CancelOrder> &) override;
  void operator()(const Event<CancelAllOrders> &) override;

  // order management
  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;
  void operator()(const Event<TradeUpdate> &) override;

  // account management
  void operator()(const Event<PositionUpdate> &) override;
  void operator()(const Event<FundsUpdate> &) override;

  // broadcast
  void operator()(const Event<CustomMetricsUpdate> &) override;

 private:
  std::unique_ptr<client::EventLogReader> reader_;
};

}  // namespace event_log
}  // namespace samples
}  // namespace roq
