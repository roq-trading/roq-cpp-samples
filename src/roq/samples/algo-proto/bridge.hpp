/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <string>
#include <vector>

#include "roq/client.hpp"

#include "roq/utils/container.hpp"

#include "roq/cache/gateway.hpp"
#include "roq/cache/market.hpp"

#include "roq/samples/algo-proto/config.hpp"
#include "roq/samples/algo-proto/route.hpp"
#include "roq/samples/algo-proto/settings.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/handler.hpp"

#include "roq/algo/strategies/factory.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Bridge final : public client::Handler, public algo::framework::Dispatcher {
  Bridge(client::Dispatcher &, Settings const &, Config const &, size_t size);

  Bridge(Bridge &&) = default;
  Bridge(Bridge const &) = delete;

 protected:
  uint64_t next_order_id() const override { return max_order_id_ + 1; }

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
  bool find_source(uint64_t order_id, Callback);

 private:
  client::Dispatcher &dispatcher_;
  // note! we assume each {exchange,symbol} can only originate from a single source
  utils::unordered_map<std::string, utils::unordered_map<std::string, Route>> routes_;
  std::vector<cache::Gateway> gateways_;
  utils::unordered_map<std::string, std::unique_ptr<algo::framework::Factory>> factories_;
  utils::unordered_map<uint32_t, std::unique_ptr<algo::framework::Handler>> strategies_;
  utils::unordered_map<std::string, uint32_t> routing_id_to_strategy_;
  uint32_t strategy_id_ = {};
  uint64_t max_order_id_ = {};
  // note! we must maintain a map of order_id to source
  utils::unordered_map<uint64_t, uint8_t> order_id_to_source_;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
