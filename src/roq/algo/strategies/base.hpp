/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <string_view>
#include <vector>

#include "roq/string.hpp"

#include "roq/utils/container.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/handler.hpp"
#include "roq/algo/framework/state.hpp"

#include "roq/algo/strategies/order_manager.hpp"

namespace roq {
namespace algo {
namespace strategies {

struct Base : public framework::Handler {
  Base(framework::Dispatcher &, framework::State const &, std::string_view const &routing_id, CreateOrder const &);

  Base(Base &&) = default;

  Base(Base const &) = delete;

  bool ready() const { return ready_; }

  template <typename... Args>
  auto extract(size_t index, Args &&...args) const {
    auto &market = static_cast<cache::Market const &>(state_.get_market(index));
    return (*market.market_by_price).extract(std::forward<Args>(args)...);
  }

  uint64_t next_order_id() { return dispatcher_.next_order_id(); }

 protected:
  struct Ready final {};
  struct NotReady final {};

  virtual void operator()(Ready const &) = 0;
  virtual void operator()(NotReady const &) = 0;

  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<GatewaySettings> const &) override;
  void operator()(Event<GatewayStatus> const &) override;

  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;

  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;

  template <typename T>
  void dispatch(Event<T> const &);

 public:
  framework::Dispatcher &dispatcher_;
  framework::State const &state_;
  RoutingId const routing_id_;
  Account const account_;
  std::vector<OrderManager> order_managers_;

 protected:
  friend OrderManager;
  utils::unordered_map<uint64_t, size_t> order_id_to_order_manager_index_;

 private:
  template <typename T>
  void update(Event<T> const &);

  std::vector<bool> ready_by_instrument_;
  bool ready_ = false;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
