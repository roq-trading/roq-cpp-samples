/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

#include "roq/samples/example-3/instrument.h"
#include "roq/samples/example-3/model.h"

namespace roq {
namespace samples {
namespace example_3 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<Timer> &) override;
  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<GatewayStatus> &) override;
  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;
  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;
  void operator()(const Event<TradeUpdate> &) override;
  void operator()(const Event<PositionUpdate> &) override;
  void operator()(const Event<FundsUpdate> &) override;

  // helper - dispatch event to instrument
  template <typename T>
  void dispatch(const T &event) {
    assert(event.message_info.source == 0);
    instrument_(event.value);
  }

  void update_model();

  void try_trade(Side, double price);

 private:
  client::Dispatcher &dispatcher_;
  Instrument instrument_;
  uint32_t max_order_id_ = {};
  Model model_;
  std::chrono::nanoseconds next_sample_ = {};
  uint32_t working_order_id_ = {};
  Side working_side_ = {};
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
