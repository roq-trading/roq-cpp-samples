/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

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
  Strategy(const Strategy &) = delete;

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
  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<GatewaySettings> &) override;
  void operator()(const Event<StreamStatus> &) override;
  void operator()(const Event<ExternalLatency> &) override;
  void operator()(const Event<GatewayStatus> &) override;
  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<MarketByOrderUpdate> &) override;
  void operator()(const Event<TradeSummary> &) override;

 private:
  client::Dispatcher &dispatcher_;
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq
