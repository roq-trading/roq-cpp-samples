/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_6 {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  // host
  void operator()(Event<Timer> const &) override;
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;

  // control
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;

  // config
  void operator()(Event<GatewaySettings> const &) override;

  // stream
  void operator()(Event<StreamStatus> const &) override;

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

  // order management
  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;
  void operator()(Event<TradeUpdate> const &) override;

  // account management
  void operator()(Event<PositionUpdate> const &) override;
  void operator()(Event<FundsUpdate> const &) override;

  template <typename T>
  void check(Event<T> const &);

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  std::chrono::nanoseconds prev_receive_time_ = {};
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
