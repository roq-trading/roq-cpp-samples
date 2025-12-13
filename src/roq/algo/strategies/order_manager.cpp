/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/algo/strategies/order_manager.hpp"

#include "roq/logging.hpp"

#include "roq/algo/strategies/base.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace strategies {

// TODO(thraneh): Base -> shared (lookup + dispatcher + ...) + instrument

// === IMPLEMENTATION ===

OrderManager::OrderManager(Base &base, size_t index, Side const &side) : base_{base}, index_{index}, side_{side} {
}

void OrderManager::set_target(double quantity, double price) {
  assert(base_.ready());
  auto changed = false;
  changed |= utils::update(target_quantity_, quantity);
  changed |= utils::update(target_price_, price);
  if (!changed) {
    return;
  }
  log::debug("UPDATE!"sv);
  if (order_id_ == 0) {
    log::debug("CREATE_ORDER"sv);
    auto order_id = base_.dispatcher_.next_order_id();
    auto &instrument = base_.state_.get_instrument(index_);
    auto create_order = CreateOrder{
        .account = base_.account_,
        .order_id = order_id,
        .exchange = instrument.exchange,
        .symbol = instrument.symbol,
        .side = side_,
        .position_effect = {},
        .margin_mode = {},
        .max_show_quantity = NaN,
        .order_type = OrderType::LIMIT,
        .time_in_force = TimeInForce::GTC,
        .execution_instructions = {},
        .request_template = {},
        .quantity = target_quantity_,
        .price = target_price_,
        .stop_price = NaN,
        .leverage = NaN,
        .routing_id = base_.routing_id_,
        .strategy_id = {},
    };
    base_.dispatcher_(create_order);
    order_id_ = order_id;
    [[maybe_unused]] auto res = base_.order_id_to_order_manager_index_.try_emplace(order_id, index_);
    assert(res.second);
  } else {
    log::debug("MODIFY_ORDER"sv);
    // XXX assuming it's target price
    auto modify_order = ModifyOrder{
        .account = base_.account_,
        .order_id = order_id_,
        .request_template = {},
        .quantity = NaN,
        .price = target_price_,
        .routing_id = base_.routing_id_,
        .version = {},
        .conditional_on_version = {},
    };
    base_.dispatcher_(modify_order);
  }
}

void OrderManager::start() {
}

void OrderManager::stop() {
}

void OrderManager::operator()(Event<OrderAck> const &event) {
  auto &[message_info, order_ack] = event;
  log::debug("order_ack={}"sv, order_ack);
  // XXX rejected?
}

void OrderManager::operator()(Event<OrderUpdate> const &event) {
  auto &[message_info, order_update] = event;
  log::debug("order_update={}"sv, order_update);
  // XXX finished?
}

void OrderManager::operator()(Event<PositionUpdate> const &) {
}

}  // namespace strategies
}  // namespace algo
}  // namespace roq
