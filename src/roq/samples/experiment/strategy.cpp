/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/experiment/strategy.hpp"

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"
#include "roq/utils/update.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace experiment {

// === HELPERS ===

namespace {
auto create_limit_order_request([[maybe_unused]] auto &settings, auto side, auto quantity, auto price) -> CreateOrder {
  return {
      .account = {},   // note! managed
      .order_id = {},  // note! managed
      .exchange = {},  // note! managed
      .symbol = {},    // note! managed
      .side = side,
      .position_effect = {},
      .margin_mode = {},
      .quantity_type = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {ExecutionInstruction::PARTICIPATE_DO_NOT_INITIATE},  // don't cross the market
      .request_template = {},
      .quantity = quantity,
      .price = price,
      .stop_price = NaN,
      .routing_id = {},
      .strategy_id = {},
  };
}

auto create_modify_order_request([[maybe_unused]] auto &settings, auto price) -> ModifyOrder {
  return {
      .account = {},   // note! managed
      .order_id = {},  // note! managed
      .request_template = {},
      .quantity = NaN,
      .price = price,
      .routing_id = {},
      .version = {},                 // note! managed
      .conditional_on_version = {},  // note! managed
  };
}

auto create_cancel_order_request([[maybe_unused]] auto &settings) -> CancelOrder {
  return {
      .account = {},   // note! managed
      .order_id = {},  // note! managed
      .request_template = {},
      .routing_id = {},
      .version = {},                 // note! managed
      .conditional_on_version = {},  // note! managed
  };
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(Shared &shared)
    : shared_{shared}, leg_1_{shared_, Side::BUY, shared_.settings.quantity}, leg_2_{shared_, Side::SELL, shared_.settings.quantity} {
}

void Strategy::operator()(Event<Disconnected> const &) {
  ready_ = false;
}

void Strategy::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Strategy::operator()(Event<ReferenceData> const &, Market const &market) {
  maybe_create_orders(market);
}

void Strategy::operator()(Event<MarketStatus> const &, Market const &market) {
  maybe_create_orders(market);
}

void Strategy::operator()(Event<TopOfBook> const &, Market const &) {
  if (leg_1_.ready() && leg_2_.ready()) {
    start();
  }
}

void Strategy::operator()(Event<OrderUpdate> const &, Order const &) {
  auto done_1 = leg_1_.done();
  auto done_2 = leg_2_.done();
  if (done_1 && done_2) {
    stop();
  } else if (done_1 && !done_2) {
    auto price = (2.0 * shared_.settings.price_low + shared_.settings.price_high) / 3.0;
    leg_2_.modify(price);
  } else if (!done_1 && done_2) {
    auto price = (shared_.settings.price_low + 2 * shared_.settings.price_high) / 3.0;
    leg_1_.modify(price);
  }
}

// state machine

void Strategy::start() {
  leg_1_.create(shared_.settings.price_low);
  leg_2_.create(shared_.settings.price_high);
}

void Strategy::stop() {
  shared_.dispatcher.stop();
}

// -

void Strategy::maybe_create_orders(Market const &market) {
  leg_1_(market);
  leg_2_(market);
  if (leg_1_.ready() && leg_2_.ready()) {
    start();
  }
}

// Leg

Strategy::Leg::Leg(Shared &shared, Side side, double quantity) : shared_{shared}, side_{side}, quantity_{quantity} {
}

bool Strategy::Leg::ready() const {
  return state_ == State::READY;
}

bool Strategy::Leg::done() const {
  return state_ == State::DONE;
}

void Strategy::Leg::operator()(Market const &market) {
  if (order_) [[likely]] {
    return;
  }
  assert(state_ == State::UNDEFINED);
  // create an order management object
  // notice that we can attach order and trade update handlers so we can later receive the callbacks
  auto order_update_handler = [this](auto &event, auto &order) { update(event, order); };
  order_ = shared_.dispatcher.create_order(shared_.settings.account, market, order_update_handler);
  state_ = State::READY;
}

void Strategy::Leg::create(double price) {
  assert(order_);
  assert(state_ == State::READY);
  assert(std::isnan(price_));
  assert(!std::isnan(price));
  price_ = price;
  state_ = State::CREATING;
  create_helper(0);
}

// remember to throttle
void Strategy::Leg::create_helper(size_t retry_counter) {
  assert(order_);
  assert(state_ == State::CREATING);
  auto failure_handler = [this, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CREATING);
    if (retry_counter >= shared_.settings.max_retries) {
      log::fatal("Too many retries"sv);
    }
    auto &[message_info, order_ack] = event;
    if (order_ack.error == Error::REQUEST_RATE_LIMIT_REACHED) {
      auto timer_handler = [this, retry_counter]([[maybe_unused]] auto &event) { create_helper(retry_counter + 1); };
      shared_.dispatcher.add_timer(shared_.settings.retry_delay, timer_handler);
    } else {
      state_ = State::FAILED;
      log::fatal("Unexpected: error={}"sv, order_ack.error);
    }
  };
  auto success_handler = [this, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CREATING);
    auto &[message_info, order_ack] = event;
    if (!utils::is_equal(order_ack.price, price_)) {
      state_ = State::MODIFYING;
      modify_helper(0);
    } else {
      state_ = State::WORKING;
    }
  };
  auto request = create_limit_order_request(shared_.settings, side_, quantity_, price_);
  (*order_)(request, failure_handler, success_handler);
}

// remember to throttle
void Strategy::Leg::modify(double price) {
  assert(order_);
  assert(!std::isnan(price_));
  assert(!std::isnan(price));
  if (utils::update(price_, price)) {
    switch (state_) {
      using enum State;
      case UNDEFINED:
      case READY:
        assert(false);
        break;
      case CREATING:
      case MODIFYING:
        // will be handled by the response callbacks
        break;
      case WORKING:
        state_ = State::MODIFYING;
        modify_helper(0);
        break;
      case DONE:
      case FAILED:
        log::warn("Unexpected: is this a race condition?"sv);  // XXX TODO unsure about this
        break;
    }
  }
}

void Strategy::Leg::modify_helper(size_t retry_counter) {
  switch (state_) {
    using enum State;
    case UNDEFINED:
    case READY:
    case CREATING:
      assert(false);  // should never happen
#ifndef NDEBUG
      log::fatal("Unexpected"sv);
#endif
      break;
    case MODIFYING:
      break;
    case WORKING:
      assert(false);  // forgot to set state before calling this function?
#ifndef NDEBUG
      log::fatal("Unexpected"sv);
#endif
      break;
    case DONE:
    case FAILED:
      return;  // possible due to async updates
  }
  auto failure_handler = [this, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::MODIFYING);
    if (retry_counter >= shared_.settings.max_retries) {
      log::fatal("Too many retries"sv);
    }
    auto &[message_info, order_ack] = event;
    if (order_ack.error == Error::REQUEST_RATE_LIMIT_REACHED) {
      auto timer_handler = [this, retry_counter]([[maybe_unused]] auto &event) { modify_helper(retry_counter + 1); };
      shared_.dispatcher.add_timer(shared_.settings.retry_delay, timer_handler);
    } else if (order_ack.error == Error::TOO_LATE_TO_MODIFY_OR_CANCEL) {
      state_ = State::DONE;
    } else {
      state_ = State::FAILED;
      log::fatal("Unexpected: error={}"sv, order_ack.error);
    }
  };
  auto success_handler = [this, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::MODIFYING);
    auto &[message_info, order_ack] = event;
    if (!utils::is_equal(order_ack.price, price_)) {
      state_ = State::MODIFYING;
      modify_helper(0);
    } else {
      state_ = State::WORKING;
    }
  };
  auto request = create_modify_order_request(shared_.settings, price_);
  (*order_)(request, failure_handler, success_handler);
}

void Strategy::Leg::update(Event<OrderUpdate> const &event, Order const &) {
  auto &[message_info, order_update] = event;
  // note! here we don't have to check for rejected because the order-ack will have triggered the failure handler
  if (order_update.order_status == OrderStatus::COMPLETED) {
    state_ = State::DONE;
  }
}

}  // namespace experiment
}  // namespace samples
}  // namespace roq
