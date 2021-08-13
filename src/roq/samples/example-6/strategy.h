/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_6 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  // host
  void operator()(const Event<Timer> &) override;
  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;

  // control
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;

  // config
  void operator()(const Event<GatewaySettings> &) override;

  // stream
  void operator()(const Event<StreamStatus> &) override;

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

  // order management
  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;
  void operator()(const Event<TradeUpdate> &) override;

  // account management
  void operator()(const Event<PositionUpdate> &) override;
  void operator()(const Event<FundsUpdate> &) override;

  template <typename T>
  void check(const Event<T> &);

 private:
  client::Dispatcher &dispatcher_;
  std::chrono::nanoseconds prev_receive_time_ = {};
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
