/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-8/strategy.h"

#include <algorithm>

#include "roq/logging.h"

#include "roq/utils/compare.h"

#include "roq/samples/example-8/flags.h"

using namespace std::literals;
using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_8 {

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher),
      market_by_price_(client::DepthBuilderFactory::create(Flags::exchange(), Flags::symbol())) {
}

void Strategy::operator()(const Event<Timer> &event) {
  auto &[message_info, timer] = event;
  if (active_ && next_info_ < timer.now) {
    next_info_ = timer.now + 5s;
    log::info(
        "best_bid={}, best_ask={}, limit_price={}, quantity_when_placed={}, traded_since_placed={}"sv,
        best_bid_,
        best_ask_,
        limit_price_,
        quantity_when_placed_,
        traded_since_placed_);
  }
}

void Strategy::operator()(const Event<GatewaySettings> &event) {
  (*market_by_price_)(event);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  (*market_by_price_)(event);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  (*market_by_price_)(event);
  auto bids = (*market_by_price_).bids();
  if (!std::empty(bids)) {
    best_bid_ = (*market_by_price_).internal_to_price(bids[0].first);
    if (!active_) {
      active_ = true;
      auto index = Flags::tick_offset();
      if (std::size(bids) >= index) {
        limit_price_ = (*market_by_price_).internal_to_price(bids[index].first);
        quantity_when_placed_ = (*market_by_price_).internal_to_quantity(bids[index].second);
        log::info("Placed: limit_price={}, quantity_when_place={}"sv, limit_price_, quantity_when_placed_);
      }
    }
  }
  auto asks = (*market_by_price_).asks();
  if (!std::empty(asks)) {
    best_ask_ = (*market_by_price_).internal_to_price(asks[0].first);
    if (active_) {
      auto limit_price = (*market_by_price_).price_to_internal(limit_price_);
      if (asks[0].second && asks[0].first <= limit_price) {
        log::info("COMPLETED (ask <= bid)"sv);
        active_ = false;
        dispatcher_.stop();
      }
    }
  }
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  auto &[message_info, trade_summary] = event;
  log::info("trade_summary={}"sv, trade_summary);
  if (active_) {
    bool updated = false;
    // note! we can't cache this because the scaling may change
    auto limit_price = (*market_by_price_).price_to_internal(limit_price_);
    for (auto &trade : trade_summary.trades) {
      auto price = (*market_by_price_).price_to_internal(trade.price);
      if (price == limit_price) {
        traded_since_placed_ += trade.quantity;
        updated = true;
      }
    }
    if (updated) {
      auto quantity_in_front = std::max(0.0, quantity_when_placed_ - traded_since_placed_);
      log::info("Changed: traded_since_placed={}, quantity_in_front={}"sv, traded_since_placed_, quantity_in_front);
      if (utils::compare(quantity_in_front, 0.0) == 0) {
        log::info("COMPLETED (filled)"sv);
        active_ = false;
        dispatcher_.stop();
      }
    }
  }
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
