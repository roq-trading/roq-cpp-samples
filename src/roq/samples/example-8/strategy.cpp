/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-8/strategy.hpp"

#include <algorithm>

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"

#include "roq/samples/example-8/flags.hpp"

using namespace std::literals;
using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_{dispatcher}, account_{Flags::account()}, exchange_{Flags::exchange()}, symbol_{Flags::symbol()},
      quantity_{Flags::quantity()}, tick_offset_{Flags::tick_offset()},
      market_by_price_(client::MarketByPriceFactory::create(exchange_, symbol_)) {
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

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  // log::info("LATENCY={}"sv, message_info.receive_time - message_info.origin_create_time);
  // log::info("message_info={}"sv, message_info);
  (*market_by_price_)(market_by_price_update);
  // warmup?
  if (wait_.count()) {
    if (message_info.receive_time < wait_)
      return;
  } else {
    wait_ = message_info.receive_time + 30s;
    return;
  }
  // order already sent?
  if (done_)
    return;
  // incremental?
  if (market_by_price_update.update_type != UpdateType::INCREMENTAL)
    return;
  // ready?
  if (utils::compare(tick_size_, 0.0) == 0)
    return;
  // extract top of book
  std::array<Layer, 1> tmp;
  auto top_of_book = (*market_by_price_).extract(tmp, true);
  if (std::empty(top_of_book))
    return;
  // send order
  auto price = top_of_book[0].bid_price - tick_offset_ * tick_size_;
  CreateOrder create_order{
      .account = account_,
      .order_id = ++order_id_,
      .exchange = exchange_,
      .symbol = symbol_,
      .side = Side::BUY,
      .position_effect = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {},
      .order_template = {},
      .quantity = quantity_,
      .price = price,
      .stop_price = NaN,
      .routing_id = {},
  };
  log::info("create_order={}"sv, create_order);
  dispatcher_.send(create_order, 0u, true);
  done_ = true;
}

void Strategy::operator()(Event<OrderAck> const &event) {
  auto &[message_info, order_ack] = event;
  log::info("LATENCY={}"sv, message_info.receive_time - message_info.origin_create_time);
  log::info("message_info={}"sv, message_info);
  log::info("order_ack={}"sv, order_ack);
  log::fatal("STOP"sv);
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
