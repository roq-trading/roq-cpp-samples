/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

#include "roq/samples/example-3/instrument.h"
#include "roq/samples/example-3/model.h"

namespace roq {
namespace samples {
namespace example_3 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher);

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

 protected:
  void operator()(const Event<Timer>& event) override;
  void operator()(const Event<Connection>& event) override;
  void operator()(const Event<DownloadBegin>& event) override;
  void operator()(const Event<DownloadEnd>& event) override;
  void operator()(const Event<MarketDataStatus>& event) override;
  void operator()(const Event<OrderManagerStatus>& event) override;
  void operator()(const Event<ReferenceData>& event) override;
  void operator()(const Event<MarketStatus>& event) override;
  void operator()(const Event<MarketByPriceUpdate>& event) override;
  void operator()(const Event<OrderAck>& event) override;
  void operator()(const Event<OrderUpdate>& event) override;
  void operator()(const Event<TradeUpdate>& event) override;
  void operator()(const Event<PositionUpdate>& event) override;
  void operator()(const Event<FundsUpdate>& event) override;

  // helper - dispatch event to instrument
  template <typename T>
  void dispatch(const T& event) {
    assert(event.message_info.source == uint8_t{0});
    _instrument(event.value);
  }

  void update_model();

  void try_trade(Side side, double price);

 private:
  client::Dispatcher& _dispatcher;
  Instrument _instrument;
  uint32_t _max_order_id = 0;
  Model _model;
  std::chrono::nanoseconds _next_sample = {};
  uint32_t _working_order_id = 0;
  Side _working_side = Side::UNDEFINED;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
