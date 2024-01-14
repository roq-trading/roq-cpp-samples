/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-8/strategy.hpp"

#include <algorithm>

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"

using namespace std::literals;
using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher, Settings const &settings)
    : dispatcher_{dispatcher}, account_{settings.account}, exchange_{settings.exchange}, symbol_{settings.symbol},
      use_top_of_book_{settings.top_of_book}, use_market_by_price_{!use_top_of_book_}, quantity_{settings.quantity},
      tick_offset_{settings.tick_offset}, market_by_price_(client::MarketByPriceFactory::create(exchange_, symbol_)) {
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  log::info("event={}"sv, event);
  auto &[message_info, download_begin] = event;
  if (account_.compare(download_begin.account) == 0)
    downloading_ = true;
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  log::info("event={}"sv, event);
  auto &[message_info, download_end] = event;
  if (account_.compare(download_end.account) == 0) {
    order_id_ = std::max(order_id_, download_end.max_order_id);
    downloading_ = false;
  }
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  auto &[message_info, reference_data] = event;
  tick_size_ = reference_data.tick_size;
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  // trigger?
  if (!use_top_of_book_)
    return;
  // order action
  create_order(message_info, top_of_book.layer);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  // trigger?
  if (!use_market_by_price_)
    return;
  // apply mbp update
  (*market_by_price_)(market_by_price_update);
  // incremental?
  if (market_by_price_update.update_type != UpdateType::INCREMENTAL)
    return;
  // extract top of book
  auto layers = (*market_by_price_).extract(buffer_, true);
  if (std::empty(layers))
    return;
  // order action
  create_order(message_info, layers[0]);
}

void Strategy::operator()(Event<OrderAck> const &event) {
  auto &[message_info, order_ack] = event;
  log::info("LATENCY={}"sv, message_info.receive_time - message_info.origin_create_time);
}

void Strategy::create_order(MessageInfo const &message_info, Layer const &layer) {
  // done?
  if (countdown_ == 0)
    return;
  // ready?
  if (next_request_.count()) {
    if (message_info.receive_time < next_request_)
      return;
  } else {
    next_request_ = message_info.receive_time + 30s;  // warmup
    return;
  }
  // tick size ?
  if (utils::compare(tick_size_, 0.0) == 0)
    return;
  // send order
  auto price = layer.bid_price - tick_offset_ * tick_size_;
  auto create_order = CreateOrder{
      .account = account_,
      .order_id = ++order_id_,
      .exchange = exchange_,
      .symbol = symbol_,
      .side = Side::BUY,
      .position_effect = {},
      .margin_mode = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {},
      .request_template = {},
      .quantity = quantity_,
      .price = price,
      .stop_price = NaN,
      .routing_id = {},
      .strategy_id = {},
  };
  log::info<1>("create_order={}"sv, create_order);
  dispatcher_.send(create_order, 0u, true);
  // update
  assert(countdown_ > 0);
  --countdown_;
  next_request_ = message_info.receive_time + 100ms;
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
