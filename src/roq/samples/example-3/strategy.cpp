/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/strategy.h"

#include <limits>

#include "roq/logging.h"

#include "roq/samples/example-3/options.h"
#include "roq/samples/example-3/utilities.h"

namespace roq {
namespace samples {
namespace example_3 {

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher),
      instrument_(FLAGS_exchange, FLAGS_symbol, FLAGS_account) {
}

void Strategy::operator()(const Event<Timer> &event) {
  // note! using system clock (*not* the wall clock)
  if (event.value.now < next_sample_) return;
  if (next_sample_.count())  // initialized?
    update_model();
  auto now = std::chrono::duration_cast<std::chrono::seconds>(event.value.now);
  next_sample_ = now + std::chrono::seconds{FLAGS_sample_freq_secs};
  // possible extension: reset request timeout
}

void Strategy::operator()(const Event<Connection> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  dispatch(event);
  if (update(max_order_id_, event.value.max_order_id)) {
    LOG(INFO)(R"(max_order_id={})", max_order_id_);
  }
}

void Strategy::operator()(const Event<MarketDataStatus> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<OrderManagerStatus> &event) {
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
  LOG(INFO)(R"(OrderAck={})", event.value);
  auto &order_ack = event.value;
  if (is_request_complete(order_ack.status)) {
    // possible extension: reset request timeout
  }
}

void Strategy::operator()(const Event<OrderUpdate> &event) {
  LOG(INFO)(R"(OrderUpdate={})", event.value);
  dispatch(event);  // update position
  auto &order_update = event.value;
  if (is_order_complete(order_update.status)) {
    working_order_id_ = 0;
    working_side_ = Side::UNDEFINED;
  }
}

void Strategy::operator()(const Event<TradeUpdate> &event) {
  LOG(INFO)(R"(TradeUpdate={})", event.value);
}

void Strategy::operator()(const Event<PositionUpdate> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<FundsUpdate> &event) {
  LOG(INFO)(R"(FundsUpdate={})", event.value);
}

void Strategy::update_model() {
  if (instrument_.is_ready()) {
    auto side = model_.update(instrument_);
    switch (side) {
      case Side::UNDEFINED:
        // nothing to do
        break;
      case Side::BUY: try_trade(side, instrument_.best_bid()); break;
      case Side::SELL: try_trade(side, instrument_.best_ask()); break;
    }
  } else {
    model_.reset();
  }
}

void Strategy::try_trade(Side side, double price) {
  if (FLAGS_enable_trading == false) {
    LOG(WARNING)("Trading *NOT* enabled");
    return;
  }
  // if buy:
  //   if sell order outstanding
  //     cancel old order
  //   if position not long
  //     send buy order
  //
  if (working_order_id_) {
    LOG(INFO)("*** ANOTHER ORDER IS WORKING ***");
    if (side != working_side_) {
      LOG(INFO)("*** CANCEL WORKING ORDER ***");
      dispatcher_.send(
          CancelOrder{.account = FLAGS_account, .order_id = working_order_id_},
          uint8_t{0});
    }
    return;
  }
  if (instrument_.can_trade(side) == false) {
    LOG(INFO)("*** CAN'T INCREASE POSITION ***");
    return;
  }
  auto order_id = ++max_order_id_;
  dispatcher_.send(
      CreateOrder{
          .account = FLAGS_account,
          .order_id = order_id,
          .exchange = FLAGS_exchange,
          .symbol = FLAGS_symbol,
          .side = side,
          .quantity = instrument_.min_trade_vol(),
          .order_type = OrderType::LIMIT,
          .price = price,
          .time_in_force = TimeInForce::GTC,
          .position_effect = PositionEffect::UNDEFINED,
          .execution_instruction = ExecutionInstruction::UNDEFINED,
          .stop_price = std::numeric_limits<double>::quiet_NaN(),
          .max_show_quantity = std::numeric_limits<double>::quiet_NaN(),
          .order_template = {},
      },
      uint8_t{0});
  working_order_id_ = order_id;
  working_side_ = side;
  // possible extension: monitor for request timeout
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
