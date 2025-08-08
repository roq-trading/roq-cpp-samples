/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/client/event_log_reader.hpp"

namespace roq {
namespace samples {
namespace event_log {

struct Processor final : public client::EventLogReader::Handler {
  explicit Processor(std::string_view const &path);

  Processor(Processor &&) = default;
  Processor(Processor const &) = delete;

  void dispatch();

 protected:
  void operator()(Event<Control> const &) override;
  void operator()(Event<ControlAck> const &) override;
  void operator()(Event<ServiceUpdate> const &) override;
  void operator()(Event<StrategyUpdate> const &) override;
  void operator()(Event<LegsUpdate> const &) override;

  // config
  void operator()(Event<GatewaySettings> const &) override;

  // stream
  void operator()(Event<StreamStatus> const &) override;
  void operator()(Event<ExternalLatency> const &) override;
  void operator()(Event<RateLimitsUpdate> const &) override;
  void operator()(Event<RateLimitTrigger> const &) override;

  // service
  void operator()(Event<GatewayStatus> const &) override;

  // market data
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<MarketByOrderUpdate> const &) override;
  void operator()(Event<TradeSummary> const &) override;
  void operator()(Event<StatisticsUpdate> const &) override;
  void operator()(Event<TimeSeriesUpdate> const &) override;

  // order actions (from client, note! source == user)
  void operator()(Event<CreateOrder> const &) override;
  void operator()(Event<ModifyOrder> const &) override;
  void operator()(Event<CancelOrder> const &) override;
  void operator()(Event<CancelAllOrders> const &) override;

  // order management
  void operator()(Event<CancelAllOrdersAck> const &) override;
  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;
  void operator()(Event<TradeUpdate> const &) override;

  // account management
  void operator()(Event<PositionUpdate> const &) override;
  void operator()(Event<FundsUpdate> const &) override;

  // broadcast
  void operator()(Event<CustomMetricsUpdate> const &) override;
  void operator()(Event<CustomMatrixUpdate> const &) override;

  // parameters
  void operator()(Event<ParametersUpdate> const &) override;

  // position manager
  void operator()(Event<PortfolioUpdate> const &) override;

  // risk management
  void operator()(Event<RiskLimits> const &) override;
  void operator()(Event<RiskLimitsUpdate> const &) override;

  // market making
  void operator()(Event<MassQuote> const &) override;
  void operator()(Event<MassQuoteAck> const &) override;
  void operator()(Event<CancelQuotes> const &) override;
  void operator()(Event<CancelQuotesAck> const &) override;

 private:
  std::unique_ptr<client::EventLogReader> reader_;
};

}  // namespace event_log
}  // namespace samples
}  // namespace roq
