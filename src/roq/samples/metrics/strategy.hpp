/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/utils/metrics/counter.hpp"
#include "roq/utils/metrics/histogram.hpp"

namespace roq {
namespace samples {
namespace metrics {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = delete;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<ExternalLatency> const &) override;
  void operator()(roq::metrics::Writer &) const override;

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  // note! following variables are each aligned to a cache line
  roq::utils::metrics::Counter<uint64_t> counter_;
  roq::utils::metrics::external_latency_t histogram_;  // note! or internal_latency_t or your own...
};

}  // namespace metrics
}  // namespace samples
}  // namespace roq
