/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/strategy-1/controller.hpp"

#include "roq/clock.hpp"
#include "roq/logging.hpp"

#include "roq/utils/update.hpp"

#include "roq/io/sys/scheduler.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace strategy_1 {

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
      shared_{settings_, *this}, strategy_{shared_}, bridge_{strategy::Bridge::create(strategy_, *this)} {
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
  auto timer = Timer{
      .now = now,
  };
  create_event_and_dispatch(*bridge_, message_info, timer);
}

void Controller::add_timer(std::chrono::nanoseconds delay, strategy::Bridge::TimerHandler const &timer_handler) {
  return (*bridge_).add_timer(delay, timer_handler);
}

std::unique_ptr<strategy::Order> Controller::create_order(
    std::string_view const &account, strategy::Market const &market, strategy::Order::Handler const &handler) {
  return (*bridge_).create_order(account, market, handler);
}

std::unique_ptr<strategy::Order> Controller::create_order(
    std::string_view const &account, strategy::Market const &market, strategy::Bridge::OrderUpdateHandler const &order_update_handler) {
  return (*bridge_).create_order(account, market, order_update_handler);
}

std::unique_ptr<strategy::Order> Controller::create_order(
    std::string_view const &account,
    strategy::Market const &market,
    strategy::Bridge::OrderUpdateHandler const &order_update_handler,
    strategy::Bridge::TradeUpdateHandler const &trade_update_handler) {
  return (*bridge_).create_order(account, market, order_update_handler, trade_update_handler);
}

bool Controller::has_account(uint8_t source, std::string_view const &account) const {
  return (*bridge_).has_account(source, account);
}

strategy::Market const &Controller::get_market(std::string_view const &exchange, std::string_view const &symbol) const {
  return (*bridge_).get_market(exchange, symbol);
}

bool Controller::has_market(std::string_view const &exchange, std::string_view const &symbol) const {
  return (*bridge_).has_market(exchange, symbol);
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
  dispatch(event);
}

void Controller::operator()(Event<DownloadEnd> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<Ready> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<ReferenceData> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<OrderAck> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<OrderUpdate> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<TradeUpdate> const &event) {
  dispatch(event);
}

// outbound

void Controller::operator()(CreateOrder const &create_order, uint8_t source) {
  log::debug("create_order={}, source={}"sv, create_order, source);
  (*dispatcher_).send(create_order, source);
}

void Controller::operator()(ModifyOrder const &modify_order, uint8_t source) {
  log::debug("modify_order={}, source={}"sv, modify_order, source);
  (*dispatcher_).send(modify_order, source);
}

void Controller::operator()(CancelOrder const &cancel_order, uint8_t source) {
  log::debug("cancel_order={}, source={}"sv, cancel_order, source);
  (*dispatcher_).send(cancel_order, source);
}

template <typename T, typename... Args>
void Controller::dispatch(Event<T> const &event, Args &&...args) {
  (*bridge_)(event);
}

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
