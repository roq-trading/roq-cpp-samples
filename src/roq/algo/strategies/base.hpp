/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>
#include <vector>

#include "roq/string.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/handler.hpp"
#include "roq/algo/framework/state.hpp"

#include "roq/algo/strategies/order_manager.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Base : public framework::Handler {
 public:
  Base(framework::Dispatcher &, const framework::State &, const std::string_view &routing_id, const CreateOrder &);

  Base(Base &&) = default;

  Base(const Base &) = delete;

  bool ready() const { return ready_; }

  template <typename... Args>
  auto extract(size_t index, Args &&...args) const {
    auto &market = static_cast<const cache::Market &>(state_.get_market(index));
    return (*market.market_by_price).extract(std::forward<Args>(args)...);
  }

  uint32_t next_order_id() { return dispatcher_.next_order_id(); }

 protected:
  struct Ready final {};
  struct NotReady final {};

  virtual void operator()(const Ready &) = 0;
  virtual void operator()(const NotReady &) = 0;

  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  void operator()(const Event<DownloadBegin> &) override;
  void operator()(const Event<DownloadEnd> &) override;
  void operator()(const Event<GatewaySettings> &) override;
  void operator()(const Event<GatewayStatus> &) override;

  void operator()(const Event<ReferenceData> &) override;
  void operator()(const Event<MarketStatus> &) override;

  void operator()(const Event<OrderAck> &) override;
  void operator()(const Event<OrderUpdate> &) override;

  template <typename T>
  void dispatch(const Event<T> &);

 public:
  framework::Dispatcher &dispatcher_;
  const framework::State &state_;
  const RoutingId routing_id_;
  const Account account_;
  std::vector<OrderManager> order_managers_;

 protected:
  friend OrderManager;
  absl::flat_hash_map<uint32_t, size_t> order_id_to_order_manager_index_;

 private:
  template <typename T>
  void update(const Event<T> &);

  std::vector<bool> ready_by_instrument_;
  bool ready_ = false;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
