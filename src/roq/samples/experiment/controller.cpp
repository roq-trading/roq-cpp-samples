/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/experiment/controller.hpp"

#include "roq/clock.hpp"
#include "roq/logging.hpp"

#include "roq/io/sys/scheduler.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace experiment {

// === CONSTANTS ===

namespace {
auto const DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK = 1000uz;
auto const WAIT_THIS_LONG_BEFORE_NEXT_STATE_CHANGE = 10s;
auto const YIELD_FREQUENCY = 1000ms;
auto const TIMER_FREQUENCY = 100ms;

}  // namespace

// === HELPERS ===

namespace {
auto create_dispatcher(auto &settings, auto &config, auto &context, auto &connections) {
  return client::Poller::create(settings, config, context, connections);
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(Settings const &settings, Config const &config, io::Context &context, std::span<std::string_view const> const &params)
    : settings_{settings}, terminate_{context.create_signal(*this, io::sys::Signal::Type::TERMINATE)},
      interrupt_{context.create_signal(*this, io::sys::Signal::Type::INTERRUPT)}, dispatcher_{create_dispatcher(settings, config, context, params)},
      source_(std::numeric_limits<uint8_t>::max()), strategy_{*this, settings_}, handler_{strategy_} {
}

void Controller::dispatch() {
  log::info("Starting the dispatch loop..."sv);
  (*dispatcher_).start();
  auto ok = true;
  while (ok) {
    auto now = clock::get_system();
    if (next_timer_ < now) {
      auto now_utc = clock::get_realtime();
      auto message_info = MessageInfo{
          .source = SOURCE_SELF,
          .source_name = {},
          .source_session_id = {},
          .source_seqno = {},
          .receive_time_utc = now_utc,
          .receive_time = now,
          .source_send_time = now,
          .source_receive_time = now,
          .origin_create_time = now,
          .origin_create_time_utc = now_utc,
          .is_last = true,
          .opaque = {},
      };
      refresh(now);
      next_timer_ = now + TIMER_FREQUENCY;
    }
    if (next_yield_ < now && YIELD_FREQUENCY.count() > 0) {
      next_yield_ = now + YIELD_FREQUENCY;
      io::sys::Scheduler::yield();
    }
    for (size_t i = 0; ok && i < DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK; ++i) {
      ok = (*dispatcher_).dispatch(*this);
    }
  }
  log::info("The dispatch loop has stopped!"sv);
}

void Controller::refresh(std::chrono::nanoseconds now) {
  if (std::empty(timer_queue_)) {
    return;
  }
  auto now_utc = clock::get_realtime();
  auto value = now.count();
  auto key = timer_queue_.top();
  if (value < key) {
    return;
  }
  auto iter = timer_handlers_.find(key);
  if (iter == std::end(timer_handlers_)) {
    log::fatal("Unexpected"sv);
  }
  for (auto &handler : (*iter).second) {
    try {
      auto message_info = MessageInfo{
          .source = SOURCE_SELF,
          .source_name = {},
          .source_session_id = {},
          .source_seqno = {},
          .receive_time_utc = now_utc,
          .receive_time = now,
          .source_send_time = now,
          .source_receive_time = now,
          .origin_create_time = now,
          .origin_create_time_utc = now_utc,
          .is_last = true,
          .opaque = {},
      };
      auto timer = Timer{
          .now = now,
      };
      create_event_and_dispatch(handler, message_info, timer);
    } catch (...) {
      // XXX TODO ???
    }
  }
  timer_handlers_.erase(iter);
  timer_queue_.pop();
}

void Controller::stop() {
}

std::unique_ptr<Order> Controller::create_order(
    std::string_view const &account, Market const &market, OrderUpdateHandler const &order_update_handler, TradeUpdateHandler const &trade_update_handler) {
  return std::make_unique<Order>(*this, account, market, order_update_handler, trade_update_handler);
}

void Controller::add_timer(std::chrono::nanoseconds delay, TimerHandler const &timer_handler) {
  auto now = clock::get_system();  // XXX TODO careful here / doesn't work with simulation
  auto timeout = now + delay;
  auto key = timeout.count();
  timer_queue_.emplace(key);
  timer_handlers_[key].emplace_back(timer_handler);
}

// io::sys::Signal::Handler

void Controller::operator()(io::sys::Signal::Event const &event) {
  log::warn("*** SIGNAL: {} ***"sv, event.type);
  (*dispatcher_).stop();
}

// client::Poller::Handler

void Controller::operator()(Event<Connected> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<Disconnected> const &event) {
  auto &[message_info, disconnected] = event;
  auto &source = source_[message_info.source];
  assert(source.ready);
  source.ready = false;
  for (auto &[order_id, order] : source.order_lookup) {
    (*order)(event);
  }
  source.accounts.clear();
  for (auto &[exchange, tmp] : exchange_symbol_to_market_) {
    for (auto &[symbol, market] : tmp) {
      market(event);
    }
  }
  dispatch(event);
}

void Controller::operator()(Event<DownloadEnd> const &event) {
  auto &[message_info, download_end] = event;
  auto &source = source_[message_info.source];
  if (source.max_order_id < download_end.max_order_id) {
    source.max_order_id = download_end.max_order_id;
  }
  source.accounts.emplace(download_end.account);
}

void Controller::operator()(Event<Ready> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<ReferenceData> const &event) {
  dispatch_market(event);
}

void Controller::operator()(Event<MarketStatus> const &event) {
  dispatch_market(event);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  dispatch_market(event);
}

void Controller::operator()(Event<OrderAck> const &event) {
  dispatch_order(event);
}

void Controller::operator()(Event<OrderUpdate> const &event) {
  dispatch_order(event);
}

void Controller::operator()(Event<TradeUpdate> const &event) {
  dispatch_order(event);
}

// outbound

void Controller::operator()(CreateOrder const &create_order, uint8_t source) {
  log::warn("create_order={}, source={}"sv, create_order, source);
  (*dispatcher_).send(create_order, source);
}

void Controller::operator()(ModifyOrder const &modify_order, uint8_t source) {
  (*dispatcher_).send(modify_order, source);
}

void Controller::operator()(CancelOrder const &cancel_order, uint8_t source) {
  (*dispatcher_).send(cancel_order, source);
}

// helpers

void Controller::create_order_mapping(Order &order) {
  assert(order.source != SOURCE_NONE);
  if (!order.order_id_) {
    auto &source = source_[order.source];
    auto order_id = ++source.max_order_id;
    [[maybe_unused]] auto [iter, res] = source.order_lookup.try_emplace(order_id, &order);
    assert(res);
    order.order_id_ = order_id;  // note! after insert
    log::debug("Created order_id={} (source={}, max_order_id={})"sv, order.order_id_, order.source, source.max_order_id);
  } else {
    assert(false);  // unexpected
  }
}

void Controller::remove_order_mapping(Order &order, bool rollback) {
  assert(order.source != SOURCE_NONE);
  if (!order.order_id_) {
    return;
  }
  auto &source = source_[order.source];
  if (rollback) {
    assert(order.order_id_ == source.max_order_id);
    assert(source.max_order_id > 0);
    --source.max_order_id;
  }
  log::debug("Removed order_id={} (source={}, max_order_id={})"sv, order.order_id_, order.source, source.max_order_id);
  auto iter = source.order_lookup.find(order.order_id_);
  if (iter != std::end(source.order_lookup)) {
    assert((*iter).second == &order);
    source.order_lookup.erase(iter);
    order.order_id_ = {};  // note! after remove
    order.version_ = {};
  } else {
    assert(false);  // unexpected
  }
}

template <typename T, typename... Args>
void Controller::dispatch(Event<T> const &event, Args &&...args) {
  handler_(event, std::forward<Args>(args)...);
}

template <typename T>
void Controller::dispatch_market(Event<T> const &event) {
  auto &market = get_market(event);
  market(event);
  dispatch(event, market);
}

template <typename T>
void Controller::dispatch_order(Event<T> const &event) {
  auto &order = get_order(event);
  order(event);
  dispatch(event, order);
}

template <typename T>
Market &Controller::get_market(Event<T> const &event) {
  auto &[message_info, value] = event;
  auto &tmp = exchange_symbol_to_market_[value.exchange];
  auto iter = tmp.find(value.symbol);
  if (iter == std::end(tmp)) [[unlikely]] {
    iter = tmp.try_emplace(value.symbol, message_info.source, value.exchange, value.symbol).first;
  } else if ((*iter).second.source != message_info.source) [[unlikely]] {
    log::fatal(
        R"(Unexpected: exchange="{}", symbol="{}" is provided by both source={} and source={})"sv,
        value.exchange,
        value.symbol,
        message_info.source,
        (*iter).second.source);
  }
  return (*iter).second;
}

Market const &Controller::get_market(std::string_view const &exchange, std::string_view const &symbol) const {
  auto iter_1 = exchange_symbol_to_market_.find(exchange);
  if (iter_1 == std::end(exchange_symbol_to_market_)) {
    throw RuntimeError{R"(There is no market for exchange="{}")"sv, exchange};
  }
  auto &tmp = (*iter_1).second;
  auto iter_2 = tmp.find(symbol);
  if (iter_2 == std::end(tmp)) {
    throw RuntimeError{R"(There is no market for exchange="{}", symbol="{}")"sv, exchange, symbol};
  }
  return (*iter_2).second;
}

bool Controller::has_market(std::string_view const &exchange, std::string_view const &symbol) const {
  auto iter_1 = exchange_symbol_to_market_.find(exchange);
  if (iter_1 == std::end(exchange_symbol_to_market_)) {
    return false;
  }
  auto &tmp = (*iter_1).second;
  auto iter_2 = tmp.find(symbol);
  return iter_2 != std::end(tmp);
}

template <typename T>
Order &Controller::get_order(Event<T> const &event) {
  auto &[message_info, value] = event;
  auto &source = source_[message_info.source];
  auto iter = source.order_lookup.find(value.order_id);
  if (iter == std::end(source.order_lookup)) [[unlikely]] {
    log::fatal("Unexpected: source={}, order_id={}"sv, message_info.source, value.order_id);
  }
  return *(*iter).second;
}

bool Controller::has_account(uint8_t source, std::string_view const &account) const {
  auto &accounts = source_[source].accounts;
  return accounts.find(account) != std::end(accounts);
}

}  // namespace experiment
}  // namespace samples
}  // namespace roq
