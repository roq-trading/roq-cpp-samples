/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_4 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &dispatcher);

  Strategy(const Strategy &) = delete;
  Strategy(Strategy &&) = default;

 protected:
  void operator()(const Event<Connection> &event) override;
  void operator()(const Event<DownloadBegin> &event) override;
  void operator()(const Event<DownloadEnd> &event) override;
  void operator()(const Event<MarketDataStatus> &event) override;
  void operator()(const Event<OrderManagerStatus> &event) override;
  void operator()(const Event<ReferenceData> &event) override;
  void operator()(const Event<MarketStatus> &event) override;
  void operator()(const Event<MarketByPriceUpdate> &event) override;
  void operator()(const Event<MarketByOrderUpdate> &event) override;
  void operator()(const Event<TradeSummary> &event) override;

 private:
  client::Dispatcher &dispatcher_;
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
