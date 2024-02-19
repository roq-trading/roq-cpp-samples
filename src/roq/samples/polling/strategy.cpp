/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/polling/strategy.hpp"

#include <cassert>

#include "roq/logging.hpp"

#include "roq/clock.hpp"

#include "roq/utils/common.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace polling {

// === CONSTANTS ===

namespace {
auto const DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK = 1000uz;
auto const WAIT_THIS_LONG_BEFORE_NEXT_STATE_CHANGE = 10s;
}  // namespace

// === HELPERS ===

namespace {
auto create_dispatcher(auto &handler, auto &settings, auto &config, auto &context, auto &connections) {
  return client::Simple::create(handler, settings, config, context, connections);
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(
    Settings const &settings,
    Config const &config,
    io::Context &context,
    std::span<std::string_view const> const &connections)
    : settings_{settings}, dispatcher_{create_dispatcher(*this, settings, config, context, connections)} {
}

// XXX FIXME option to choose who owns the signal handler
void Strategy::dispatch() {
  log::info("Start the dispatch loop..."sv);
  (*dispatcher_).start();
  auto ok = true;
  while (ok) {
    auto now = clock::get_system();
    // XXX FIXME yield
    refresh(now);
    for (size_t i = 0; ok && i < DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK; ++i)
      ok = (*dispatcher_).dispatch();
  }
  log::info("Dispatch loop has stopped!"sv);
}

void Strategy::operator()(State state) {
  assert(state_ != state);
  state_ = state;
  log::info("state={}"sv, magic_enum::enum_name(state_));
  auto now = clock::get_system();
  next_update_ = now + WAIT_THIS_LONG_BEFORE_NEXT_STATE_CHANGE;
}

void Strategy::refresh(std::chrono::nanoseconds now) {
  if (now < next_update_)
    return;
  switch (state_) {
    using enum State;
    case UNDEFINED:
      assert(next_update_.count() == 0);
      (*this)(State::CONNECTING);
      break;
    case CONNECTING:
      break;
    case READY:
      (*this)(State::CREATE_ORDER);
      break;
    case CREATE_ORDER:
      (*this)(State::WAITING_CREATE);
      create_order();
      break;
    case WAITING_CREATE:
      log::warn("Request has timed out"sv);
      (*this)(State::DONE);
      break;
    case WORKING:
      (*this)(State::CANCEL_ORDER);
      break;
    case CANCEL_ORDER:
      (*this)(State::WAITING_CANCEL);
      cancel_order();
      break;
    case WAITING_CANCEL:
      log::warn("Request has timed out"sv);
      (*this)(State::DONE);
      break;
    case DONE:
      (*dispatcher_).stop();
      break;
  }
}

void Strategy::create_order() {
  assert(order_id_ == 0);
  order_id_ = ++max_order_id_;
  auto create_order = CreateOrder{
      .account = settings_.account,
      .order_id = order_id_,
      .exchange = settings_.exchange,
      .symbol = settings_.symbol,
      .side = Side::BUY,
      .position_effect = {},
      .margin_mode = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {},
      .request_template = {},
      .quantity = settings_.quantity,
      .price = settings_.price,
      .stop_price = NaN,
      .routing_id = {},
      .strategy_id = {},
  };
  try {
    (*dispatcher_).send(create_order, 0);
  } catch (NotConnected const &e) {
    log::fatal("{}"sv, e);
  } catch (NotReady const &e) {
    log::fatal("{}"sv, e);
  }
}

void Strategy::cancel_order() {
  assert(order_id_ != 0);
  auto cancel_order = CancelOrder{
      .account = settings_.account,
      .order_id = order_id_,
      .request_template = {},
      .routing_id = {},
      .version = {},
      .conditional_on_version = {},
  };
  try {
    (*dispatcher_).send(cancel_order, 0);
  } catch (NotConnected const &e) {
    log::fatal("{}"sv, e);
  } catch (NotReady const &e) {
    log::fatal("{}"sv, e);
  }
}

// client::Simple::Handler

void Strategy::operator()(Event<Connected> const &event) {
  log::debug("event={}"sv, event);
  assert(state_ == State::CONNECTING);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  log::debug("event={}"sv, event);
  (*this)(State::CONNECTING);
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  log::debug("event={}"sv, event);
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  log::debug("event={}"sv, event);
  auto &download_end = event.value;
  auto max_order_id = download_end.max_order_id;
  if (max_order_id_ < max_order_id) {
    max_order_id_ = max_order_id;
    log::info("max_order_id={}"sv, max_order_id_);
  }
}

void Strategy::operator()(Event<Ready> const &event) {
  log::debug("event={}"sv, event);
  assert(state_ == State::CONNECTING);
  (*this)(State::READY);
}

void Strategy::operator()(Event<GatewaySettings> const &) {
}

void Strategy::operator()(Event<StreamStatus> const &) {
}

void Strategy::operator()(Event<GatewayStatus> const &) {
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  // log::debug("event={}"sv, event);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  // log::debug("event={}"sv, event);
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  // log::debug("event={}"sv, event);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  // log::debug("event={}"sv, event);
}

void Strategy::operator()(Event<OrderAck> const &event) {
  log::debug("event={}"sv, event);
  auto &order_ack = event.value;
  // waiting?
  if (!utils::has_request_maybe_completed(order_ack.request_status))
    return;
  // failed?
  if (utils::has_request_failed(order_ack.request_status)) {
    log::warn("Request has failed: status={}"sv, order_ack.request_status);
    (*this)(State::DONE);
    return;
  }
  // success?
  switch (order_ack.request_type) {
    using enum RequestType;
    case UNDEFINED:
      log::fatal("Unexpected"sv);
      break;
    case CREATE_ORDER:
      (*this)(State::WORKING);
      break;
    case MODIFY_ORDER:
      log::fatal("Unexpected"sv);
      break;
    case CANCEL_ORDER:
      (*this)(State::DONE);
      break;
  }
}

void Strategy::operator()(Event<OrderUpdate> const &event) {
  log::debug("event={}"sv, event);
}

}  // namespace polling
}  // namespace samples
}  // namespace roq
