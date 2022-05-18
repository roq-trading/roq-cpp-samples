/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_1 {

// client::Handler is the base class managing the life-time of
// a strategy (could be in-process simulation or connectivity
// to one or more market access gateways)
// the constructor must *always* accept a client::Dispatcher as the
// first argument
// the construct *may* accept more arguments from the 2nd position
// event handlers *may* be overriden

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  // the following event handlers log every update as-is
  // an important lesson here is to only use only verbose logging in
  // the "hot path" (i.e. inside the event handlers)
  // using log::info(...) is not disallowed, but you should only use
  // this logging facility very minimally as it may have undesirable
  // latency impact
  // note!
  //   the ROQ_v environment variable controls the verbosity level
  //   for example, "export ROQ_v=1"
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<GatewaySettings> const &) override;
  void operator()(Event<StreamStatus> const &) override;
  void operator()(Event<ExternalLatency> const &) override;
  void operator()(Event<RateLimitTrigger> const &) override;
  void operator()(Event<GatewayStatus> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<MarketByOrderUpdate> const &) override;
  void operator()(Event<TradeSummary> const &) override;
  void operator()(Event<CustomMetricsUpdate> const &) override;

 private:
  client::Dispatcher &dispatcher_;
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq
