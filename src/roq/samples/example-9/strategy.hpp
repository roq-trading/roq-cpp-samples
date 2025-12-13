/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/cache/time_series.hpp"

#include "roq/samples/example-9/settings.hpp"

namespace roq {
namespace samples {
namespace example_9 {

struct Strategy final : public client::Handler {
  Strategy(client::Dispatcher &, Settings const &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Timer> const &) override;
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<Ready> const &) override;
  void operator()(Event<TimeSeriesUpdate> const &) override;

  void process(std::span<Bar const> const &, UpdateType);

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  cache::TimeSeries time_series_;
};

}  // namespace example_9
}  // namespace samples
}  // namespace roq
