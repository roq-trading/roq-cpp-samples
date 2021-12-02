/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_4 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<GatewayStatus> &) override;
  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<MarketByOrderUpdate> &) override;
  void operator()(const Event<TradeSummary> &) override;

 private:
  client::Dispatcher &dispatcher_;
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
