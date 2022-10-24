/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-2/instrument.hpp"

#include <fmt/format.h>

#include "roq/logging.hpp"

#include "roq/mask.hpp"

#include "roq/utils/compare.hpp"
#include "roq/utils/update.hpp"

#include "roq/samples/example-2/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_2 {

// === IMPLEMENTATION ===

Instrument::Instrument(std::string_view const &exchange, std::string_view const &symbol)
    : exchange_{exchange}, symbol_{symbol}, market_by_price_{client::MarketByPriceFactory::create(exchange, symbol)} {
}

void Instrument::operator()(Connected const &) {
  if (utils::update(connected_, true)) {
    log::info("[{}:{}] connected={}"sv, exchange_, symbol_, connected_);
    check_ready();
  }
}

void Instrument::operator()(Disconnected const &) {
  if (utils::update(connected_, false)) {
    log::info("[{}:{}] connected={}"sv, exchange_, symbol_, connected_);
    // reset all cached state - await download upon reconnection
    reset();
  }
}

void Instrument::operator()(DownloadBegin const &download_begin) {
  if (!std::empty(download_begin.account))  // we only care about market (not account)
    return;
  assert(!download_);
  download_ = true;
  log::info("[{}:{}] download={}"sv, exchange_, symbol_, download_);
}

void Instrument::operator()(DownloadEnd const &download_end) {
  if (!std::empty(download_end.account))  // we only care about market (not account)
    return;
  assert(download_);
  download_ = false;
  log::info("[{}:{}] download={}"sv, exchange_, symbol_, download_);
  // update the ready flag
  check_ready();
}

void Instrument::operator()(GatewayStatus const &gateway_status) {
  if (!std::empty(gateway_status.account))  // we only care about market (not account)
    return;
  // bit-mask of required message types
  static const Mask required{
      SupportType::REFERENCE_DATA,
      SupportType::MARKET_STATUS,
      SupportType::MARKET_BY_PRICE,
  };
  // readiness defined by full availability of all required message types
  auto market_data = gateway_status.available.has_all(required) && gateway_status.unavailable.has_none(required);
  if (utils::update(market_data_, market_data))
    log::info("[{}:{}] market_data={}"sv, exchange_, symbol_, market_data_);
  // sometimes useful to see what is missing
  if (!market_data_) {
    auto missing = required & ~gateway_status.available;
    log::debug("missing={}"sv, missing);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(ReferenceData const &reference_data) {
  assert(exchange_.compare(reference_data.exchange) == 0);
  assert(symbol_.compare(reference_data.symbol) == 0);
  // update our cache
  if (utils::update(tick_size_, reference_data.tick_size)) {
    log::info("[{}:{}] tick_size={}"sv, exchange_, symbol_, tick_size_);
  }
  if (utils::update(min_trade_vol_, reference_data.min_trade_vol)) {
    log::info("[{}:{}] min_trade_vol={}"sv, exchange_, symbol_, min_trade_vol_);
  }
  if (utils::update(multiplier_, reference_data.multiplier)) {
    log::info("[{}:{}] multiplier={}"sv, exchange_, symbol_, multiplier_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(MarketStatus const &market_status) {
  assert(exchange_.compare(market_status.exchange) == 0);
  assert(symbol_.compare(market_status.symbol) == 0);
  // update our cache
  if (utils::update(trading_status_, market_status.trading_status)) {
    log::info("[{}:{}] trading_status={}"sv, exchange_, symbol_, trading_status_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(MarketByPriceUpdate const &market_by_price_update) {
  assert(exchange_.compare(market_by_price_update.exchange) == 0);
  assert(symbol_.compare(market_by_price_update.symbol) == 0);
  if (download_)
    log::info("MarketByPriceUpdate={}"sv, market_by_price_update);
  // update depth
  // note!
  //   market by price only gives you *changes*.
  //   you will most likely want to use the the price to look up
  //   the relative position in an order book and then modify the
  //   liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  (*market_by_price_)(market_by_price_update);
  auto depth = (*market_by_price_).extract(depth_, true);
  log::info<1>("[{}:{}] depth=[{}]"sv, exchange_, symbol_, fmt::join(depth_, ", "sv));
  if (std::size(depth) > 0 && is_ready())
    update_model();
}

void Instrument::update_model() {
  // one sided market?
  if (utils::is_zero(depth_[0].bid_quantity) || utils::is_zero(depth_[0].ask_quantity))
    return;
  // validate depth
  auto spread = depth_[0].ask_price - depth_[0].bid_price;
  if (utils::is_less_or_equal(spread, 0.0))
    log::fatal(
        "[{}:{}] Probably something wrong: "
        "choice price or price inversion detected. "
        "depth=[{}]"sv,
        exchange_,
        symbol_,
        fmt::join(depth_, ", "sv));
  // compute (weighted) mid
  double sum_1 = 0.0, sum_2 = 0.0;
  for (auto &[bid_price, bid_quantity, ask_price, ask_quantity] : depth_) {
    sum_1 += bid_price * bid_quantity + ask_price * ask_quantity;
    sum_2 += bid_quantity + ask_quantity;
  }
  mid_price_ = sum_1 / sum_2;
  // update (exponential) moving average
  if (std::isnan(avg_price_))
    avg_price_ = mid_price_;  // initialize
  else
    avg_price_ = Flags::alpha() * mid_price_ + (1.0 - Flags::alpha()) * avg_price_;
  // only verbose logging
  log::info<1>("[{}:{}] model={{mid_price={}, avg_price={}}}"sv, exchange_, symbol_, mid_price_, avg_price_);
}

void Instrument::check_ready() {
  auto before = ready_;
  ready_ = connected_ && !download_ && utils::is_greater(tick_size_, 0.0) && utils::is_greater(min_trade_vol_, 0.0) &&
           utils::is_greater(multiplier_, 0.0) && trading_status_ == TradingStatus::OPEN && market_data_;
  if (ready_ != before)
    log::info("[{}:{}] ready={}"sv, exchange_, symbol_, ready_);
}

void Instrument::reset() {
  connected_ = false;
  download_ = false;
  tick_size_ = NaN;
  min_trade_vol_ = NaN;
  trading_status_ = {};
  market_data_ = {};
  (*market_by_price_).clear();
  mid_price_ = NaN;
  avg_price_ = NaN;
  ready_ = false;
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
