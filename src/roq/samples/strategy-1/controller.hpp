/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

#include "roq/api.hpp"

#include "roq/io/sys/signal.hpp"

#include "roq/client/poller.hpp"

#include "roq/strategy/bridge.hpp"

#include "roq/samples/strategy-1/config.hpp"
#include "roq/samples/strategy-1/settings.hpp"

#include "roq/samples/strategy-1/shared.hpp"
#include "roq/samples/strategy-1/strategy.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

struct Controller final : public io::sys::Signal::Handler, public client::Poller::Handler, public strategy::Bridge::Dispatcher {
  Controller(Settings const &, Config const &, io::Context &, std::span<std::string_view const> const &);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

  void dispatch();

  // bridge

  void add_timer(std::chrono::nanoseconds delay, strategy::Bridge::TimerHandler const &);

  std::unique_ptr<strategy::Order> create_order(std::string_view const &account, strategy::Market const &, strategy::Bridge::OrderUpdateHandler const &);

  std::unique_ptr<strategy::Order> create_order(
      std::string_view const &account, strategy::Market const &, strategy::Bridge::OrderUpdateHandler const &, strategy::Bridge::TradeUpdateHandler const &);

  bool has_account(uint8_t source, std::string_view const &account) const;

  strategy::Market const &get_market(std::string_view const &exchange, std::string_view const &symbol) const;

  bool has_market(std::string_view const &exchange, std::string_view const &symbol) const;

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

  // strategy::Bridge::Dispatcher
  void operator()(CreateOrder const &, uint8_t source) override;
  void operator()(ModifyOrder const &, uint8_t source) override;
  void operator()(CancelOrder const &, uint8_t source) override;

  void refresh(std::chrono::nanoseconds now);

  template <typename T, typename... Args>
  void dispatch(Event<T> const &, Args &&...);

 private:
  Settings const &settings_;
  std::unique_ptr<io::sys::Signal> terminate_;
  std::unique_ptr<io::sys::Signal> interrupt_;
  std::unique_ptr<client::Poller> dispatcher_;
  std::chrono::nanoseconds next_yield_ = {};
  std::chrono::nanoseconds next_timer_ = {};
  Shared shared_;
  //
  Strategy strategy_;
  //
  std::unique_ptr<strategy::Bridge> bridge_;
};

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
