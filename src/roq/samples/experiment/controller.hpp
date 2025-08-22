/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

#include "roq/api.hpp"

#include "roq/io/sys/signal.hpp"

#include "roq/client/poller.hpp"

#include "roq/execution/bridge.hpp"

#include "roq/samples/experiment/config.hpp"
#include "roq/samples/experiment/settings.hpp"

#include "roq/samples/experiment/shared.hpp"
#include "roq/samples/experiment/strategy.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Controller final : public io::sys::Signal::Handler, public client::Poller::Handler, public execution::Bridge::Dispatcher {
  Controller(Settings const &, Config const &, io::Context &, std::span<std::string_view const> const &);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

  void dispatch();

  // bridge

  void add_timer(std::chrono::nanoseconds delay, execution::Bridge::TimerHandler const &);

  std::unique_ptr<execution::Order> create_order(std::string_view const &account, execution::Market const &, execution::Bridge::OrderUpdateHandler const &);

  std::unique_ptr<execution::Order> create_order(
      std::string_view const &account, execution::Market const &, execution::Bridge::OrderUpdateHandler const &, execution::Bridge::TradeUpdateHandler const &);

  bool has_account(uint8_t source, std::string_view const &account) const;

  execution::Market const &get_market(std::string_view const &exchange, std::string_view const &symbol) const;

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

  // execution::Bridge::Dispatcher
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
  std::unique_ptr<execution::Bridge> bridge_;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
