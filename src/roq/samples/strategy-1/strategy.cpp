/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/strategy-1/strategy.hpp"

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"
#include "roq/utils/update.hpp"

#include "roq/samples/strategy-1/controller.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace strategy_1 {

// === HELPERS ===

namespace {
auto create_limit_order_request(auto &settings, auto side, auto quantity, auto price) -> CreateOrder {
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
      .leverage = NaN,
      .routing_id = {},
      .strategy_id = settings.strategy_id,
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

Strategy::Strategy(Shared &shared) : shared_{shared} {
}

// strategy::Bridge::Handler

// note!
//   we create two managed orders when the market becomes known to the framework
//   creating a managed order is *not* the same as creating an order on the exchange, this will happen later

void Strategy::operator()(Event<AddMarket> const &, strategy::Market const &market) {
  assert(market.exchange == shared_.settings.exchange);
  assert(market.symbol == shared_.settings.symbol);

  high_ = std::make_unique<Leg>(shared_, market, Side::SELL, shared_.settings.quantity, shared_.settings.price_high);
  low_ = std::make_unique<Leg>(shared_, market, Side::BUY, shared_.settings.quantity, shared_.settings.price_low);
}

// note!
//   we check the status of the two managed orders, if either is filled (completed), we will try to modify the price of the other
//   although this handler will receive updates for *any* order, we don't have to route the updates because each of the
//   two managed orders will have set up their own handler

void Strategy::operator()(Event<OrderUpdate> const &, strategy::Order const &) {
  auto high = (*high_).finished();
  auto low = (*low_).finished();

  if (high && low) {
    // stop(); // XXX TODO we're done
  } else if (high && !low) {
    auto price = (shared_.settings.price_low + 2 * shared_.settings.price_high) / 3.0;  // 1/3 pull-back from high
    (*low_).modify_order(price);
  } else if (!high && low) {
    auto price = (2.0 * shared_.settings.price_low + shared_.settings.price_high) / 3.0;  // 1/3 pull-back from low
    (*high_).modify_order(price);
  }
}

// Leg

// note!
//   this creates an order manager (*not* the actual order on the exchange, see the submit_order method)

Strategy::Leg::Leg(Shared &shared, strategy::Market const &market, Side side, double quantity, double price)
    : strategy::Market::Subscriber{*this, market}, shared_{shared}, order_{shared_.controller.create_order(shared_.settings.account, market, *this)},
      side_{side}, quantity_{quantity}, price_{price} {
  assert(order_);
  assert(side_ != Side::UNDEFINED);
  assert(!std::isnan(quantity_));
  assert(!std::isnan(price_));
  assert(state_ == State::UNDEFINED);
}

bool Strategy::Leg::finished() const {
  return state_ == State::FINISHED;
}

// note!
//   following market handlers are used to trigger the initial order submission

void Strategy::Leg::operator()(Event<Ready> const &, strategy::Market const &market) {
  maybe_submit_order(market);
}

void Strategy::Leg::operator()(Event<ReferenceData> const &, strategy::Market const &market) {
  maybe_submit_order(market);
}

void Strategy::Leg::operator()(Event<MarketStatus> const &, strategy::Market const &market) {
  maybe_submit_order(market);
}

void Strategy::Leg::operator()(Event<TopOfBook> const &, strategy::Market const &market) {
  maybe_submit_order(market);
}

// note!
//   the order update is used to detect order completion

void Strategy::Leg::operator()(Event<OrderUpdate> const &event, strategy::Order const &) {
  auto &[message_info, order_update] = event;
  if (order_update.order_status == OrderStatus::COMPLETED) {
    state_ = State::FINISHED;
  }
}

// note!
//   here we submit an order *if*
//     (a) we haven't already started and
//     (b) the market is "ready"
//   the market does not become "ready" before
//     (a) the download procedure has complted and
//     (b) the market's trading status is "open"

void Strategy::Leg::maybe_submit_order(strategy::Market const &market) {
  if (state_ == State::UNDEFINED && market.ready && market.trading_status == TradingStatus::OPEN) {
    submit_order();
  }
}

void Strategy::Leg::submit_order() {
  assert(state_ == State::UNDEFINED);
  state_ = State::CREATING;
  submit_order_helper(0);
}

// note!
//   this helper function can be called repeatedly with an increasing retry counter following certain errors,
//   mainly rejects due to the exchange's rate limiter.
//   it is important to recognize that this function is *not* called recursively due to the asynchronous nature of
//   request and response, resubmission only happens *after* we receive an order ack with a specific error.
//   it is also important to notice that the request allows you to assign callback handlers for failure and success.
//   the callback is managed by the order object and the lifetime of the capture must therefore align with the lifetime of
//   the order object.

void Strategy::Leg::submit_order_helper(size_t retry_counter) {
  assert(state_ == State::CREATING);

  auto handle_retry = [this](auto delay, auto retry_counter) {
    assert(state_ == State::CREATING);

    if (retry_counter >= shared_.settings.max_retries) {
      log::fatal("Too many retries"sv);
    }

    auto handle_timer = [this, retry_counter]([[maybe_unused]] auto &event) { submit_order_helper(retry_counter + 1); };

    shared_.controller.add_timer(delay, handle_timer);
  };

  auto handle_failure = [this, &handle_retry, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CREATING);

    auto &[message_info, order_ack] = event;

    if (order_ack.error == Error::REQUEST_RATE_LIMIT_REACHED) {
      handle_retry(shared_.settings.retry_delay, retry_counter);
    } else {
      state_ = State::FAILED;
      log::fatal("Unexpected: error={}"sv, order_ack.error);
    }
  };

  auto handle_success = [this](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CREATING);

    auto &[message_info, order_ack] = event;

    if (!utils::is_equal(order_ack.price, price_)) {  // check if price has been modified while we were waiting for the response
      state_ = State::CHANGING;
      modify_order_helper(0);
    } else {
      state_ = State::WAITING;
    }
  };

  // need something like this

  // rate_limiter.send(try_send, failure, success);
  //
  // try-catch => don't use
  //
  // order_ack from gateway => don't use ?
  //
  // order_ack from echange => use
  //
  // async issues
  //
  // GatewayStatus ==> supports order management for account ???

  try {
    if (shared_.rate_limiter.ready()) {
      auto request = create_limit_order_request(shared_.settings, side_, quantity_, price_);
      (*order_)(request, handle_failure, handle_success);
    } else {
      handle_retry(shared_.rate_limiter.delay(), retry_counter);
    }
  } catch (NotConnected &) {
    // XXX TODO what ???
  }
}

// note!
//   same concerns as for order submission

void Strategy::Leg::modify_order(double price) {
  assert(!std::isnan(price));

  if (utils::update(price_, price)) {
    switch (state_) {
      using enum State;
      case UNDEFINED:
        assert(false);
        break;
      case CREATING:
      case CHANGING:
        // nothing to do, the response handlers will use the latest price to decide the next action
        break;
      case WAITING:
        state_ = State::CHANGING;
        modify_order_helper(0);
        break;
      case FINISHED:
      case FAILED:
        log::warn("Unexpected: is this a race condition?"sv);  // XXX TODO unsure if this is possible
        break;
    }
  }
}

void Strategy::Leg::modify_order_helper(size_t retry_counter) {
  switch (state_) {
    using enum State;
    case UNDEFINED:
    case CREATING:
#ifndef NDEBUG
      log::fatal("Unexpected"sv);
#else
      assert(false);  // should never happen
#endif
      break;
    case CHANGING:
      break;
    case WAITING:
#ifndef NDEBUG
      log::fatal("Unexpected"sv);
#else
      assert(false);  // should never happen
#endif
      break;
    case FINISHED:
    case FAILED:
      return;  // possible due to async updates
  }

  auto handle_retry = [this](auto delay, auto retry_counter) {
    assert(state_ == State::CHANGING);

    if (retry_counter >= shared_.settings.max_retries) {
      log::fatal("Too many retries"sv);
    }

    auto handle_timer = [this, retry_counter]([[maybe_unused]] auto &event) { modify_order_helper(retry_counter + 1); };

    shared_.controller.add_timer(delay, handle_timer);
  };

  auto handle_failure = [this, &handle_retry, retry_counter](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CHANGING);

    auto &[message_info, order_ack] = event;

    if (order_ack.error == Error::REQUEST_RATE_LIMIT_REACHED) {
      handle_retry(shared_.settings.retry_delay, retry_counter);
    } else if (order_ack.error == Error::TOO_LATE_TO_MODIFY_OR_CANCEL) {
      state_ = State::FINISHED;
    } else {
      state_ = State::FAILED;
      log::fatal("Unexpected: error={}"sv, order_ack.error);
    }
  };

  auto handle_success = [this](auto &event, [[maybe_unused]] auto &order) {
    assert(state_ == State::CHANGING);

    auto &[message_info, order_ack] = event;

    if (!utils::is_equal(order_ack.price, price_)) {  // check if price has been modified while we were waiting for the response
      state_ = State::CHANGING;
      modify_order_helper(0);
    } else {
      state_ = State::WAITING;
    }
  };

  try {
    if (shared_.rate_limiter.ready()) {
      auto request = create_modify_order_request(shared_.settings, price_);
      (*order_)(request, handle_failure, handle_success);
    } else {
      handle_retry(shared_.rate_limiter.delay(), retry_counter);
    }
  } catch (NotConnected &) {
    // XXX TODO what ???
  }
}

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
