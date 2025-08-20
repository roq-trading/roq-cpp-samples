/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

#include "roq/api.hpp"

#include "roq/io/sys/signal.hpp"

#include "roq/client/poller.hpp"

#include "roq/samples/experiment/config.hpp"
#include "roq/samples/experiment/settings.hpp"

#include "roq/samples/experiment/strategy.hpp"
#include "roq/samples/experiment/test.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Controller final : public io::sys::Signal::Handler, public client::Poller::Handler, public Dispatcher {
  Controller(Settings const &, Config const &, io::Context &, std::span<std::string_view const> const &);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

  void dispatch();

  void stop() override;

  std::unique_ptr<Order> create_order(
      std::string_view const &account, Market const &, OrderUpdateHandler const & = {}, TradeUpdateHandler const & = {}) override;

  void add_timer(std::chrono::nanoseconds delay, TimerHandler const &) override;

  Market const &get_market(std::string_view const &exchange, std::string_view const &symbol) const override;

  bool has_market(std::string_view const &exchange, std::string_view const &symbol) const override;

  bool has_account(uint8_t source, std::string_view const &account) const override;

 protected:
  // io::sys::Signal::Handler
  void operator()(io::sys::Signal::Event const &) override;

  // client::Poller::Handler
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<Ready> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<OrderAck> const &) override;
  void operator()(Event<OrderUpdate> const &) override;
  void operator()(Event<TradeUpdate> const &) override;

  friend Order;

  // outbound
  void operator()(CreateOrder const &, uint8_t source);
  void operator()(ModifyOrder const &, uint8_t source);
  void operator()(CancelOrder const &, uint8_t source);

  void create_order_mapping(Order &);
  void remove_order_mapping(Order &, bool rollback = false);

  void refresh(std::chrono::nanoseconds now);

  template <typename T, typename... Args>
  void dispatch(Event<T> const &, Args &&...);

  template <typename T>
  void dispatch_market(Event<T> const &);

  template <typename T>
  void dispatch_order(Event<T> const &);

  template <typename T>
  Market &get_market(Event<T> const &);

  template <typename T>
  Order &get_order(Event<T> const &);

 private:
  Settings const &settings_;
  std::unique_ptr<io::sys::Signal> terminate_;
  std::unique_ptr<io::sys::Signal> interrupt_;
  std::unique_ptr<client::Poller> dispatcher_;
  std::chrono::nanoseconds next_yield_ = {};
  std::chrono::nanoseconds next_timer_ = {};
  // EXPERIMENTAL
  // timer
  using TimerKey = int64_t;  // note! unordered_map doesn't like std::chrono::nanoseconds
  std::priority_queue<TimerKey, std::vector<TimerKey>, std::greater<TimerKey>> timer_queue_;
  utils::unordered_map<TimerKey, std::vector<TimerHandler>> timer_handlers_;
  // source
  struct Source final {
    bool ready = {};
    uint64_t max_order_id = {};
    utils::unordered_map<uint64_t, Order *> order_lookup;
    utils::unordered_set<std::string> accounts;
  };
  std::vector<Source> source_;
  // market
  // XXX TODO use opaque for faster lookup
  utils::unordered_map<std::string, utils::unordered_map<std::string, Market>> exchange_symbol_to_market_;
  // STRATEGY
  Strategy strategy_;
  experiment::Handler &handler_;  // note!
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
