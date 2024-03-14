/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/depth/strategy.hpp"

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"

#include "roq/market/mbp/factory.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace depth {

// === CONSTANTS ===

namespace {
auto const MULTIPLIER = 10uz;
}

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &, Settings const &settings)
    : settings_{settings}, mbp_full_{market::mbp::Factory::create(settings_.exchange, settings_.symbol)},
      mbp_depth_{market::mbp::Factory::create(settings_.exchange, settings_.symbol)},
      bids_(settings_.depth * MULTIPLIER), asks_(settings_.depth * MULTIPLIER) {
}

void Strategy::operator()(Event<Connected> const &) {
  log::info("connected"sv);
}

void Strategy::operator()(Event<Disconnected> const &) {
  log::info("disconnected"sv);
  (*mbp_full_).clear();
  (*mbp_depth_).clear();
}

void Strategy::operator()(Event<GatewaySettings> const &) {
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  auto &reference_data = event.value;
  (*mbp_full_)(reference_data);
  (*mbp_depth_)(reference_data);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &market_by_price_update = event.value;
  // note! the depth update *must* be computed before applying the update to the full book
  auto [bids, asks] = (*mbp_full_).create_depth_update(market_by_price_update, settings_.depth, bids_, asks_);
  auto depth_update = MarketByPriceUpdate{
      .stream_id = market_by_price_update.stream_id,
      .exchange = market_by_price_update.exchange,
      .symbol = market_by_price_update.symbol,
      .bids = bids,  // note!
      .asks = asks,  // note!
      .update_type = market_by_price_update.update_type,
      .exchange_time_utc = market_by_price_update.exchange_time_utc,
      .exchange_sequence = market_by_price_update.exchange_sequence,
      .sending_time_utc = market_by_price_update.sending_time_utc,
      .price_precision = market_by_price_update.price_precision,
      .quantity_precision = market_by_price_update.quantity_precision,
      .max_depth = settings_.depth,  // note!
      .checksum = {},
  };
  (*mbp_depth_)(depth_update);
  (*mbp_full_)(event.value);
  // TEST
  std::array<Layer, 1> lhs, rhs;
  (*mbp_full_).extract(lhs);
  (*mbp_depth_).extract(rhs);
  auto same = utils::compare(lhs[0].bid_price, rhs[0].bid_price) == 0 &&
              utils::compare(lhs[0].bid_quantity, rhs[0].bid_quantity) == 0 &&
              utils::compare(lhs[0].ask_price, rhs[0].ask_price) == 0 &&
              utils::compare(lhs[0].ask_quantity, rhs[0].ask_quantity) == 0;
  if (same) {
    log::info("lhs={}"sv, lhs);
  } else {
    log::warn("lhs={} != rhs={}"sv, lhs, rhs);
    ++issues_;
  }
  auto tmp = utils::compare(lhs[0].bid_price, lhs[0].ask_price);
  if (tmp == 0)
    ++choice_;
  if (tmp > 0)
    ++inversion_;
  log::info("issues={}, choice={}, inversion={}"sv, issues_, choice_, inversion_);
}

}  // namespace depth
}  // namespace samples
}  // namespace roq
