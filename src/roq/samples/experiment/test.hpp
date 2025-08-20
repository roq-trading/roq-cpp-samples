/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "roq/api.hpp"

#include "roq/utils/container.hpp"

#include "roq/client/poller.hpp"

#include "roq/samples/experiment/settings.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Order;

using TimerHandler = std::function<void(Event<Timer> const &)>;
using FailureHandler = std::function<void(Event<OrderAck> const &, Order &)>;
using SuccessHandler = std::function<void(Event<OrderAck> const &, Order &)>;
using OrderUpdateHandler = std::function<void(Event<OrderUpdate> const &, Order const &)>;
using TradeUpdateHandler = std::function<void(Event<TradeUpdate> const &, Order const &)>;

// download orders?

struct Controller;

struct Market final {
  Market(uint8_t source, std::string_view const &exchange, std::string_view const &symbol);

  void operator()(Event<Disconnected> const &);
  void operator()(Event<Ready> const &);
  void operator()(Event<ReferenceData> const &);
  void operator()(Event<MarketStatus> const &);
  void operator()(Event<TopOfBook> const &);

  bool is_ready() const;

  uint8_t const source;
  std::string const exchange;
  std::string const symbol;

  bool ready = {};

  double tick_size = NaN;
  TradingStatus trading_status = {};
  Layer top_of_book = {};
};

struct Order;

struct Response final {
  Response() = default;

  void success(SuccessHandler const &);
  Response &failure(FailureHandler const &);

 protected:
  friend Order;

  explicit Response(uint32_t version);

  void operator()(Event<OrderAck> const &, Order &);

 private:
  uint32_t version_ = {};
  FailureHandler failure_handler_;
  SuccessHandler success_handler_;
};

struct Order final {
  ~Order();

  // Order &operator()(UpdateHandler const &);

  Response &operator()(CreateOrder const &);
  Response &operator()(ModifyOrder const &);
  Response &operator()(CancelOrder const &);

  void operator()(CreateOrder const &, FailureHandler const &);
  void operator()(CreateOrder const &, FailureHandler const &, SuccessHandler const &);

  void operator()(ModifyOrder const &, FailureHandler const &);
  void operator()(ModifyOrder const &, FailureHandler const &, SuccessHandler const &);

  void operator()(CancelOrder const &, FailureHandler const &);
  void operator()(CancelOrder const &, FailureHandler const &, SuccessHandler const &);

  bool has_started() const;
  bool has_completed() const;

  bool can_trade() const;

  Market const &get_market() const;

  // XXX TODO remaining (current working)
  // XXX TODO traded (accumulated over multiple orders)

  // protected:
  friend Controller;

  Order(Controller &, std::string_view const &account, Market const &, OrderUpdateHandler const & = {}, TradeUpdateHandler const & = {});

  void operator()(Event<Disconnected> const &);
  void operator()(Event<Ready> const &);
  void operator()(Event<OrderAck> const &);
  void operator()(Event<OrderUpdate> const &);
  void operator()(Event<TradeUpdate> const &);

 private:
  Controller &controller_;

 public:
  std::string const account;
  std::string const exchange;
  std::string const symbol;
  uint8_t const source;
  double traded_quantity = 0.0;

 private:
  OrderUpdateHandler const order_update_handler_;
  TradeUpdateHandler const trade_update_handler_;
  //
  uint64_t order_id_ = {};
  uint32_t version_ = {};
  utils::unordered_map<uint32_t, Response> response_;  // XXX TODO replace with std::deque
  bool stale_ = {};
  // XXX TODO more order state variables
};

struct Handler {
  virtual void operator()(Event<Timer> const &) {}
  virtual void operator()(Event<Connected> const &) {}
  virtual void operator()(Event<Disconnected> const &) {}
  virtual void operator()(Event<Ready> const &) {}

  virtual void operator()(Event<ReferenceData> const &, Market const &) {}
  virtual void operator()(Event<MarketStatus> const &, Market const &) {}
  virtual void operator()(Event<TopOfBook> const &, Market const &) {}

  virtual void operator()(Event<OrderAck> const &, Order const &) {}
  virtual void operator()(Event<OrderUpdate> const &, Order const &) {}
  virtual void operator()(Event<TradeUpdate> const &, Order const &) {}
};

struct Dispatcher {
  virtual void stop() = 0;

  virtual std::unique_ptr<Order> create_order(
      std::string_view const &account, Market const &, OrderUpdateHandler const & = {}, TradeUpdateHandler const & = {}) = 0;

  virtual void add_timer(std::chrono::nanoseconds delay, TimerHandler const &) = 0;

  virtual Market const &get_market(std::string_view const &exchange, std::string_view const &symbol) const = 0;

  virtual bool has_market(std::string_view const &exchange, std::string_view const &symbol) const = 0;

  virtual bool has_account(uint8_t source, std::string_view const &account) const = 0;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
