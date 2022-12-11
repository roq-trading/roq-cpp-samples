/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/example-2/instrument.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// strategy implementation

class Strategy final : public client::Handler {
 public:
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

  // helper - dispatch event to the relevant instrument
  template <typename T>
  void dispatch(Event<T> const &);

 private:
  client::Dispatcher &dispatcher_;
  Instrument futures_;
  Instrument cash_;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
