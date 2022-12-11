/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <string>
#include <vector>

#include "roq/client.hpp"

#include "roq/cache/gateway.hpp"
#include "roq/cache/market.hpp"

#include "roq/samples/algo-proto/config.hpp"
#include "roq/samples/algo-proto/route.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/handler.hpp"

#include "roq/algo/strategies/factory.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

class Bridge final : public client::Handler, public algo::framework::Dispatcher {
 public:
  Bridge(client::Dispatcher &, Config const &, size_t size);

  Bridge(Bridge &&) = default;
  Bridge(Bridge const &) = delete;

 protected:
  uint32_t next_order_id() const override { return max_order_id_ + 1; }

  void operator()(CreateOrder const &) override;
  void operator()(ModifyOrder const &) override;
  void operator()(CancelOrder const &) override;

 protected:
  void operator()(Event<Timer> const &) override;

  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<GatewaySettings> const &) override;
  void operator()(Event<GatewayStatus> const &) override;

  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;

  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;

  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;
  void operator()(Event<TradeUpdate> const &) override;

  void operator()(Event<PositionUpdate> const &) override;

  template <typename T>
  void dispatch(Event<T> const &);

  void create_strategy(CreateOrder const &);

  template <typename Callback>
  bool find_strategy(std::string_view const &routing_id, Callback);

  template <typename Callback>
  bool find_source(uint32_t order_id, Callback);

 private:
  client::Dispatcher &dispatcher_;
  // note! we assume each {exchange,symbol} can only originate from a single source
  absl::flat_hash_map<Exchange, absl::flat_hash_map<Symbol, Route>> routes_;
  std::vector<cache::Gateway> gateways_;
  absl::flat_hash_map<Symbol, std::unique_ptr<algo::framework::Factory>> factories_;
  absl::flat_hash_map<uint32_t, std::unique_ptr<algo::framework::Handler>> strategies_;
  absl::flat_hash_map<RoutingId, uint32_t> routing_id_to_strategy_;
  uint32_t strategy_id_ = {};
  uint32_t max_order_id_ = {};
  // note! we must maintain a map of order_id to source
  absl::flat_hash_map<uint32_t, uint8_t> order_id_to_source_;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
