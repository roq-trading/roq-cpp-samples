/* Copyright (c) 2017-2022, Hans Erik Thrane */

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
  Bridge(client::Dispatcher &, const Config &, size_t size);

  Bridge(Bridge &&) = default;
  Bridge(const Bridge &) = delete;

 protected:
  uint32_t next_order_id() const override { return max_order_id_ + 1; }

  void operator()(const CreateOrder &) override;
  void operator()(const ModifyOrder &) override;
  void operator()(const CancelOrder &) override;

 protected:
  void operator()(const Event<Timer> &) override;

  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<GatewaySettings> &) override;
  void operator()(const Event<GatewayStatus> &) override;

  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;

  void operator()(const Event<TopOfBook> &) override;
  void operator()(const Event<MarketByPriceUpdate> &) override;

  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;
  void operator()(const Event<TradeUpdate> &) override;

  void operator()(const Event<PositionUpdate> &) override;

  template <typename T>
  void dispatch(const Event<T> &);

  void create_strategy(const CreateOrder &);

  template <typename Callback>
  bool find_strategy(const std::string_view &routing_id, Callback);

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
