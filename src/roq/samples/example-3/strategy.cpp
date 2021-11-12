/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/strategy.h"

#include <limits>

#include "roq/logging.h"

#include "roq/utils/common.h"
#include "roq/utils/update.h"

#include "roq/samples/example-3/flags.h"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_3 {

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher), instrument_(Flags::exchange(), Flags::symbol(), Flags::account()) {
}

void Strategy::operator()(const Event<Timer> &event) {
  // note! using system clock (*not* the wall clock)
  if (event.value.now < next_sample_)
    return;
  if (next_sample_ != next_sample_.zero())  // initialized?
    update_model();
  auto now = std::chrono::duration_cast<std::chrono::seconds>(event.value.now);
  next_sample_ = now + Flags::sample_freq();
  // possible extension: reset request timeout
}

void Strategy::operator()(const Event<Connected> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<Disconnected> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  dispatch(event);
  if (utils::update(max_order_id_, event.value.max_order_id)) {
    log::info("max_order_id={}"sv, max_order_id_);
  }
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<OrderAck> &event) {
  log::info("OrderAck={}"sv, event.value);
  auto &order_ack = event.value;
  if (utils::has_request_completed(order_ack.status)) {
    // possible extension: reset request timeout
  }
}

void Strategy::operator()(const Event<OrderUpdate> &event) {
  log::info("OrderUpdate={}"sv, event.value);
  dispatch(event);  // update position
  auto &order_update = event.value;
  if (utils::is_order_complete(order_update.status)) {
    working_order_id_ = {};
    working_side_ = {};
  }
}

void Strategy::operator()(const Event<TradeUpdate> &event) {
  log::info("TradeUpdate={}"sv, event.value);
}

void Strategy::operator()(const Event<PositionUpdate> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<FundsUpdate> &event) {
  log::info("FundsUpdate={}"sv, event.value);
}

void Strategy::update_model() {
  if (instrument_.is_ready()) {
    auto side = model_.update(instrument_);
    switch (side) {
      case Side::UNDEFINED:
        // nothing to do
        break;
      case Side::BUY:
        try_trade(side, instrument_.best_bid());
        break;
      case Side::SELL:
        try_trade(side, instrument_.best_ask());
        break;
    }
  } else {
    model_.reset();
  }
}

void Strategy::try_trade(Side side, double price) {
  if (!Flags::enable_trading()) {
    log::warn("Trading *NOT* enabled"sv);
    return;
  }
  // if buy:
  //   if sell order outstanding
  //     cancel old order
  //   if position not long
  //     send buy order
  //
  if (working_order_id_) {
    log::info("*** ANOTHER ORDER IS WORKING ***"sv);
    if (side != working_side_) {
      log::info("*** CANCEL WORKING ORDER ***"sv);
      dispatcher_.send(
          CancelOrder{
              .account = Flags::account(),
              .order_id = working_order_id_,
              .routing_id = {},
              .version = {},
              .conditional_on_version = {},
          },
          0);
    }
    return;
  }
  if (!instrument_.can_trade(side)) {
    log::info("*** CAN'T INCREASE POSITION ***"sv);
    return;
  }
  auto order_id = ++max_order_id_;
  dispatcher_.send(
      CreateOrder{
          .account = Flags::account(),
          .order_id = order_id,
          .exchange = Flags::exchange(),
          .symbol = Flags::symbol(),
          .side = side,
          .position_effect = {},
          .max_show_quantity = NaN,
          .order_type = OrderType::LIMIT,
          .time_in_force = TimeInForce::GTC,
          .execution_instruction = {},
          .order_template = {},
          .quantity = instrument_.min_trade_vol(),
          .price = price,
          .stop_price = NaN,
          .routing_id = {},
      },
      0);
  working_order_id_ = order_id;
  working_side_ = side;
  // possible extension: monitor for request timeout
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
