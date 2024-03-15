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

// === HELPERS ===

namespace {
auto is_same(auto &lhs, auto &rhs) {
  assert(std::size(lhs) == std::size(rhs));
  auto result = true;
  for (size_t i = 0; i < std::size(lhs) && result; ++i) {
    auto &l = lhs[i];
    auto &r = rhs[i];
    result &= utils::compare(l.bid_price, r.bid_price) == 0 && utils::compare(l.bid_quantity, r.bid_quantity) == 0 &&
              utils::compare(l.ask_price, r.ask_price) == 0 && utils::compare(l.ask_quantity, r.ask_quantity) == 0;
  }
  return result;
}

void print(auto &market_by_price) {
  std::vector<Layer> depth;
  market_by_price.extract_2(depth);
  for (size_t i = 0; i < std::size(depth); ++i)
    log::warn("  [{:4}] {}"sv, i, depth[i]);
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &, Settings const &settings)
    : settings_{settings}, mbp_full_{market::mbp::Factory::create(settings_.exchange, settings_.symbol)},
      mbp_depth_{market::mbp::Factory::create(settings_.exchange, settings_.symbol)},
      bids_(settings_.depth * MULTIPLIER), asks_(settings_.depth * MULTIPLIER), lhs_(settings_.depth),
      rhs_(settings_.depth) {
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
  log::info("market_by_price_update={}"sv, market_by_price_update);
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
  log::info("depth_update={}"sv, depth_update);
  (*mbp_depth_)(depth_update);
  (*mbp_full_)(event.value);
  log::warn("DEPTH:"sv);
  print(*mbp_depth_);
  log::warn("FULL:"sv);
  print(*mbp_full_);
  // TEST
  (*mbp_full_).extract(lhs_);
  (*mbp_depth_).extract(rhs_);
  auto same = is_same(lhs_, rhs_);
  if (same) {
    log::info("lhs=[{}]"sv, fmt::join(lhs_, ", "sv));
  } else {
    log::warn("lhs=[{}] != rhs=[{}]"sv, fmt::join(lhs_, ", "sv), fmt::join(rhs_, ", "sv));
    ++issues_;
    log::fatal("HERE"sv);
    // log::info("market_by_price_update={}"sv, market_by_price_update);
    // log::warn("FULL:"sv);
    // print(*mbp_full_);
    // log::warn("DEPTH:"sv);
    // print(*mbp_depth_);
  }
  auto tmp = utils::compare(lhs_[0].bid_price, lhs_[0].ask_price);
  if (tmp == 0)
    ++choice_;
  if (tmp > 0)
    ++inversion_;
  log::info("issues={}, choice={}, inversion={}"sv, issues_, choice_, inversion_);
}

}  // namespace depth
}  // namespace samples
}  // namespace roq
