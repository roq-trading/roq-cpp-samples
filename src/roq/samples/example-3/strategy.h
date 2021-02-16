/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

#include "roq/samples/example-3/instrument.h"
#include "roq/samples/example-3/model.h"

namespace roq {
namespace samples {
namespace example_3 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &dispatcher);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<Timer> &event) override;
  void operator()(const Event<Connection> &event) override;
  void operator()(const Event<DownloadBegin> &event) override;
  void operator()(const Event<DownloadEnd> &event) override;
  void operator()(const Event<MarketDataStatus> &event) override;
  void operator()(const Event<OrderManagerStatus> &event) override;
  void operator()(const Event<ReferenceData> &event) override;
  void operator()(const Event<MarketStatus> &event) override;
  void operator()(const Event<MarketByPriceUpdate> &event) override;
  void operator()(const Event<OrderAck> &event) override;
  void operator()(const Event<OrderUpdate> &event) override;
  void operator()(const Event<TradeUpdate> &event) override;
  void operator()(const Event<PositionUpdate> &event) override;
  void operator()(const Event<FundsUpdate> &event) override;

  // helper - dispatch event to instrument
  template <typename T>
  void dispatch(const T &event) {
    assert(event.message_info.source == 0u);
    instrument_(event.value);
  }

  void update_model();

  void try_trade(Side side, double price);

 private:
  client::Dispatcher &dispatcher_;
  Instrument instrument_;
  uint32_t max_order_id_ = {};
  Model model_;
  std::chrono::nanoseconds next_sample_ = {};
  uint32_t working_order_id_ = {};
  Side working_side_ = Side::UNDEFINED;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
