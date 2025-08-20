/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/experiment/test.hpp"

#include "roq/logging.hpp"

#include "roq/utils/common.hpp"

#include "roq/samples/experiment/controller.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace experiment {

// === IMPLEMENTATION ===

Market::Market(uint8_t source, std::string_view const &exchange, std::string_view const &symbol) : source{source}, exchange{exchange}, symbol{symbol} {
}

void Market::operator()(Event<Disconnected> const &) {
  ready = false;
  tick_size = NaN;
  trading_status = {};
  top_of_book = {};
}

void Market::operator()(Event<Ready> const &) {
  ready = true;
}

void Market::operator()(Event<ReferenceData> const &event) {
  auto &[message_info, reference_data] = event;
  if (!std::isnan(reference_data.tick_size)) {
    tick_size = reference_data.tick_size;
  }
}

void Market::operator()(Event<MarketStatus> const &event) {
  auto &[message_info, market_status] = event;
  if (market_status.trading_status != TradingStatus{}) {
    trading_status = market_status.trading_status;
  }
}

void Market::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  (*this).top_of_book = top_of_book.layer;
}

bool Market::is_ready() const {
  return trading_status == TradingStatus::OPEN && !std::isnan(top_of_book.bid_price) && !std::isnan(top_of_book.ask_price);
}

// Response

Response::Response(uint32_t version) : version_{version} {
}

Response &Response::failure(FailureHandler const &failure_handler) {
  assert(version_);
  assert(!failure_handler_);
  failure_handler_ = failure_handler;
  return *this;
}

void Response::success(SuccessHandler const &success_handler) {
  assert(version_);
  assert(!success_handler_);
  success_handler_ = success_handler;
}

void Response::operator()(Event<OrderAck> const &event, Order &order) {
  assert(version_);
  auto &[message_info, order_ack] = event;
  if (order_ack.error == Error{}) {
    if (success_handler_) {
      success_handler_(event, order);
    }
  } else {
    if (failure_handler_) {
      failure_handler_(event, order);
    }
  }
}

// Order

Order::Order(
    Controller &controller,
    std::string_view const &account,
    Market const &market,
    OrderUpdateHandler const &order_update_handler,
    TradeUpdateHandler const &trade_update_handler)
    : controller_{controller}, account{account}, exchange{market.exchange}, symbol{market.symbol}, source{market.source},
      order_update_handler_{order_update_handler}, trade_update_handler_{trade_update_handler} {
  if (std::empty(account)) {
    throw RuntimeError{R"(Invalid account="{}")"sv, account};
  }
}

Order::~Order() {
  controller_.remove_order_mapping(*this);
}

bool Order::has_started() const {
  return order_id_;
}

bool Order::has_completed() const {
  return !order_id_;  // note! automatically reset when order has reached a final state
}

bool Order::can_trade() const {
  auto &market = get_market();
  return controller_.has_account(market.source, account) && market.is_ready();
}

Market const &Order::get_market() const {
  return controller_.get_market(exchange, symbol);  // XXX TODO here we could benefit from market_id lookup
}

Response &Order::operator()(CreateOrder const &create_order) {
  if (order_id_) [[unlikely]] {
    throw RuntimeError{"An existing order_id is already being managed and hasn't yet reached a final state"sv};
  }
  assert(version_ == 0);
  auto create_order_2 = create_order;
  if (std::empty(create_order_2.account)) {
    create_order_2.account = account;
  } else if (create_order_2.account != account) {
    throw RuntimeError{R"(Invalid account="{}", expected="{}")"sv, create_order_2.account, account};
  }
  if (std::empty(create_order_2.exchange)) {
    create_order_2.exchange = exchange;
  } else if (create_order_2.exchange != exchange) {
    throw RuntimeError{R"(Invalid exchange="{}", expected="{}")"sv, create_order_2.exchange, exchange};
  }
  if (std::empty(create_order_2.symbol)) {
    create_order_2.symbol = symbol;
  } else if (create_order_2.symbol != symbol) {
    throw RuntimeError{R"(Invalid symbol="{}", expected="{}")"sv, create_order_2.symbol, symbol};
  }
  if (create_order_2.order_id == 0) {
    assert(order_id_ == 0);
    controller_.create_order_mapping(*this);
    assert(order_id_ != 0);
    create_order_2.order_id = order_id_;
  } else {
    throw RuntimeError{"Invalid order_id={}, expected=0"sv, create_order_2.order_id};
  }
  // note! remember to remove this order_id if the request can't be sent
  try {
    controller_(create_order_2, source);
    auto version = ++version_;
    Response response{version};
    auto [iter, res] = response_.emplace(version, std::move(response));
    assert(res);
    return (*iter).second;
  } catch (...) {
    controller_.remove_order_mapping(*this, true);
    throw;
  }
}

Response &Order::operator()(ModifyOrder const &modify_order) {
  if (order_id_) [[unlikely]] {
    throw RuntimeError{"An existing order_id is already being managed and hasn't yet reached a final state"sv};
  }
  assert(version_ > 0);
  auto modify_order_2 = modify_order;
  if (std::empty(modify_order_2.account)) {
    modify_order_2.account = account;
  } else if (modify_order_2.account != account) {
    throw RuntimeError{R"(Invalid account="{}", expected="{}")"sv, modify_order_2.account, account};
  }
  if (modify_order_2.version == 0) {
    assert(version_ > 0);
    modify_order_2.version = version_ + 1;
  } else {
    throw RuntimeError{"Invalid version={}, expected=0"sv, modify_order_2.version};
  }
  if (modify_order_2.conditional_on_version) {
    throw RuntimeError{"Invalid conditional_on_version={}, expected=0"sv, modify_order_2.conditional_on_version};
  }
  if (modify_order_2.order_id == 0) {
    assert(order_id_ != 0);
    modify_order_2.order_id = order_id_;
  } else {
    throw RuntimeError{"Invalid order_id={}, expected=0"sv, modify_order_2.order_id};
  }
  // note! no need to catch exceptions, there are no resource leaks here
  // XXX TODO dispatch
  assert(version_ >= 1);
  auto version = ++version_;
  Response response{version};
  auto [iter, res] = response_.emplace(version, std::move(response));
  assert(res);
  assert(modify_order_2.version == version_);
  return (*iter).second;
}

Response &Order::operator()(CancelOrder const &cancel_order) {
  if (order_id_) [[unlikely]] {
    throw RuntimeError{"An existing order_id is already being managed and hasn't yet reached a final state"sv};
  }
  assert(version_ > 0);
  auto cancel_order_2 = cancel_order;
  if (std::empty(cancel_order_2.account)) {
    cancel_order_2.account = account;
  } else if (cancel_order_2.account != account) {
    throw RuntimeError{R"(Invalid account="{}", expected="{}")"sv, cancel_order_2.account, account};
  }
  if (cancel_order_2.version == 0) {
    assert(version_ > 0);
    cancel_order_2.version = version_ + 1;
  } else {
    throw RuntimeError{"Invalid version={}, expected=0"sv, cancel_order_2.version};
  }
  if (cancel_order_2.conditional_on_version) {
    throw RuntimeError{"Invalid conditional_on_version={}, expected=0"sv, cancel_order_2.conditional_on_version};
  }
  if (cancel_order_2.order_id == 0) {
    assert(order_id_ != 0);
    cancel_order_2.order_id = order_id_;
  } else {
    throw RuntimeError{"Invalid order_id={}, expected=0"sv, cancel_order_2.order_id};
  }
  // note! no need to catch exceptions, there are no resource leaks here
  // XXX TODO dispatch
  assert(version_ >= 1);
  auto version = ++version_;
  Response response{version};
  auto [iter, res] = response_.emplace(version, std::move(response));
  assert(res);
  assert(cancel_order_2.version == version_);
  return (*iter).second;
}

void Order::operator()(Event<Disconnected> const &) {
  stale_ = true;
}

void Order::operator()(Event<Ready> const &) {
  if (stale_) {
    // XXX TODO must have been canceled if we're still stale af reconnect ==> what then
  }
}

void Order::operator()(CreateOrder const &create_order, FailureHandler const &failure_handler) {
  auto success_handler = []([[maybe_unused]] auto &order, [[maybe_unused]] auto &order_ack) {};
  (*this)(create_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(CreateOrder const &create_order, FailureHandler const &failure_handler, SuccessHandler const &success_handler) {
  (*this)(create_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(ModifyOrder const &modify_order, FailureHandler const &failure_handler) {
  auto success_handler = []([[maybe_unused]] auto &order, [[maybe_unused]] auto &order_ack) {};
  (*this)(modify_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(ModifyOrder const &modify_order, FailureHandler const &failure_handler, SuccessHandler const &success_handler) {
  (*this)(modify_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(CancelOrder const &cancel_order, FailureHandler const &failure_handler) {
  auto success_handler = []([[maybe_unused]] auto &order, [[maybe_unused]] auto &order_ack) {};
  (*this)(cancel_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(CancelOrder const &cancel_order, FailureHandler const &failure_handler, SuccessHandler const &success_handler) {
  (*this)(cancel_order).failure(failure_handler).success(success_handler);
}

void Order::operator()(Event<OrderAck> const &event) {
  auto &[message_info, order_ack] = event;
  assert(order_ack.order_id == order_id_);
  assert(order_ack.account == account);
  assert(order_ack.exchange == exchange);
  assert(order_ack.symbol == symbol);
  assert(order_ack.version != 0);
  if (!utils::has_request_maybe_completed(order_ack.request_status)) {
    return;
  }
  auto iter = response_.find(order_ack.version);
  assert(iter != std::end(response_));
  (*iter).second(event, *this);
  response_.erase(iter);
  // note! special case where the order never exists
  if (order_ack.request_type == RequestType::CREATE_ORDER && utils::has_request_failed(order_ack.request_status)) {
    controller_.remove_order_mapping(*this);
  }
}

void Order::operator()(Event<OrderUpdate> const &event) {
  auto &[message_info, order_update] = event;
  assert(order_update.order_id == order_id_);
  assert(order_update.account == account);
  assert(order_update.exchange == exchange);
  assert(order_update.symbol == symbol);
  assert(order_update.update_type != UpdateType{});
  stale_ = order_update.update_type == UpdateType::STALE;
  if (!std::isnan(order_update.last_traded_quantity)) {
    traded_quantity += order_update.last_traded_quantity;
  }
  if (order_update_handler_) {
    order_update_handler_(event, *this);  // XXX TODO try-catch?
  }
  if (utils::is_order_complete(order_update.order_status)) {
    controller_.remove_order_mapping(*this);
  }
}

void Order::operator()(Event<TradeUpdate> const &event) {
  auto &[message_info, trade_update] = event;
  assert(trade_update.order_id == order_id_);
  assert(trade_update.account == account);
  assert(trade_update.exchange == exchange);
  assert(trade_update.symbol == symbol);
  assert(trade_update.update_type != UpdateType{});
  if (trade_update_handler_) {
    trade_update_handler_(event, *this);  // XXX TODO try-catch?
  }
}

}  // namespace experiment
}  // namespace samples
}  // namespace roq
