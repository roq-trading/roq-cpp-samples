/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_4 {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<GatewayStatus> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<MarketByOrderUpdate> const &) override;
  void operator()(Event<TradeSummary> const &) override;

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
