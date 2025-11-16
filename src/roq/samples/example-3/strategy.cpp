/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-3/strategy.hpp"

#include <limits>

#include "roq/logging.hpp"

#include "roq/utils/common.hpp"
#include "roq/utils/update.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_3 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher, Settings const &settings)
    : dispatcher_{dispatcher}, settings_{settings}, instrument_{settings}, model_{settings} {
}

void Strategy::operator()(Event<Timer> const &event) {
  // note! using system clock (*not* the wall clock)
  if (event.value.now < next_sample_) {
    return;
  }
  if (next_sample_.count() != 0) {  // initialized?
    update_model();
  }
  auto now = std::chrono::duration_cast<std::chrono::seconds>(event.value.now);
  next_sample_ = now + settings_.sample_freq;
  // possible extension: reset request timeout
}

void Strategy::operator()(Event<Connected> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  dispatch(event);
  // note!
  // never recycle order_id's
  // by updating to the maximum of both, we guarantee not to recycle while either side is alive
  if (utils::update_max(max_order_id_, event.value.max_order_id)) {
    log::info("max_order_id={}"sv, max_order_id_);
  }
}

void Strategy::operator()(Event<Ready> const &) {
  // this event signals completion of the initial download
}

void Strategy::operator()(Event<GatewayStatus> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<MarketByOrderUpdate> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<OrderAck> const &event) {
  log::info("OrderAck={}"sv, event.value);
  auto &order_ack = event.value;
  if (utils::has_request_completed(order_ack.request_status)) {
    // possible extension: reset request timeout
  }
}

void Strategy::operator()(Event<OrderUpdate> const &event) {
  log::info("OrderUpdate={}"sv, event.value);
  dispatch(event);  // update position
  auto &order_update = event.value;
  if (utils::is_order_complete(order_update.order_status)) {
    working_order_id_ = {};
    working_side_ = {};
  }
}

void Strategy::operator()(Event<TradeUpdate> const &event) {
  log::info("TradeUpdate={}"sv, event.value);
}

void Strategy::operator()(Event<PositionUpdate> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<FundsUpdate> const &event) {
  log::info("FundsUpdate={}"sv, event.value);
}

// helper - dispatch event to instrument
template <typename T>
void Strategy::dispatch(Event<T> const &event) {
  // assert(event.message_info.source == 0);
  if (event.message_info.source == 0) {
    instrument_(event.value);
  }
}

void Strategy::update_model() {
  if (instrument_.is_ready()) {
    auto side = model_.update(instrument_);
    switch (side) {
      using enum Side;
      case UNDEFINED:
        // nothing to do
        break;
      case BUY:
        try_trade(side, instrument_.best_bid());
        break;
      case SELL:
        try_trade(side, instrument_.best_ask());
        break;
    }
  } else {
    model_.reset();
  }
}

void Strategy::try_trade(Side side, double price) {
  if (!settings_.enable_trading) {
    log::warn("Trading *NOT* enabled"sv);
    return;
  }
  // if buy:
  //   if sell order outstanding
  //     cancel old order
  //   if position not long
  //     send buy order
  //
  if (working_order_id_ != 0) {
    log::info("*** ANOTHER ORDER IS WORKING ***"sv);
    if (side != working_side_) {
      log::info("*** CANCEL WORKING ORDER ***"sv);
      auto cancel_order = CancelOrder{
          .account = settings_.account,
          .order_id = working_order_id_,
          .request_template = {},
          .routing_id = {},
          .version = {},
          .conditional_on_version = {},
      };
      dispatcher_.send(cancel_order, 0);
    }
    return;
  }
  if (!instrument_.can_trade(side)) {
    log::info("*** CAN'T INCREASE POSITION ***"sv);
    return;
  }
  auto order_id = ++max_order_id_;
  auto create_order = CreateOrder{
      .account = settings_.account,
      .order_id = order_id,
      .exchange = settings_.exchange,
      .symbol = settings_.symbol,
      .side = side,
      .position_effect = {},
      .margin_mode = {},
      .quantity_type = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {},
      .request_template = {},
      .quantity = instrument_.min_trade_vol(),
      .price = price,
      .stop_price = NaN,
      .leverage = NaN,
      .routing_id = {},
      .strategy_id = {},
  };
  dispatcher_.send(create_order, 0);
  working_order_id_ = order_id;
  working_side_ = side;
  // possible extension: monitor for request timeout
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
