/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-2/instrument.h"

#include "roq/compare.h"
#include "roq/format.h"
#include "roq/logging.h"
#include "roq/update.h"

#include "roq/samples/example-2/flags.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_2 {

Instrument::Instrument(const std::string_view &exchange, const std::string_view &symbol)
    : exchange_(exchange), symbol_(symbol),
      depth_builder_(client::DepthBuilderFactory::create(symbol, depth_)) {
}

void Instrument::operator()(const Connection &connection) {
  if (update(connection_status_, connection.status)) {
    log::info("[{}:{}] connection_status={}"_fmt, exchange_, symbol_, connection_status_);
    check_ready();
  }
  switch (connection_status_) {
    case ConnectionStatus::UNDEFINED:
      log::fatal("Unexpected"_sv);
      break;
    case ConnectionStatus::CONNECTED:
      // nothing to do for this implementation
      break;
    case ConnectionStatus::DISCONNECTED:
      // reset all cached state - await download upon reconnection
      reset();
      break;
  }
}

void Instrument::operator()(const DownloadBegin &download_begin) {
  if (!download_begin.account.empty())  // we only care about market (not account)
    return;
  assert(!download_);
  download_ = true;
  log::info("[{}:{}] download={}"_fmt, exchange_, symbol_, download_);
}

void Instrument::operator()(const DownloadEnd &download_end) {
  if (!download_end.account.empty())  // we only care about market (not account)
    return;
  assert(download_);
  download_ = false;
  log::info("[{}:{}] download={}"_fmt, exchange_, symbol_, download_);
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const StreamUpdate &stream_update) {
  // update our cache
  if (update(stream_status_, stream_update.status)) {
    log::info("[{}:{}] stream_update={}"_fmt, exchange_, symbol_, stream_status_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const ReferenceData &reference_data) {
  assert(exchange_.compare(reference_data.exchange) == 0);
  assert(symbol_.compare(reference_data.symbol) == 0);
  // update the depth builder
  depth_builder_->update(reference_data);
  // update our cache
  if (update(tick_size_, reference_data.tick_size)) {
    log::info("[{}:{}] tick_size={}"_fmt, exchange_, symbol_, tick_size_);
  }
  if (update(min_trade_vol_, reference_data.min_trade_vol)) {
    log::info("[{}:{}] min_trade_vol={}"_fmt, exchange_, symbol_, min_trade_vol_);
  }
  if (update(multiplier_, reference_data.multiplier)) {
    log::info("[{}:{}] multiplier={}"_fmt, exchange_, symbol_, multiplier_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketStatus &market_status) {
  assert(exchange_.compare(market_status.exchange) == 0);
  assert(symbol_.compare(market_status.symbol) == 0);
  // update our cache
  if (update(trading_status_, market_status.trading_status)) {
    log::info("[{}:{}] trading_status={}"_fmt, exchange_, symbol_, trading_status_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketByPriceUpdate &market_by_price_update) {
  assert(exchange_.compare(market_by_price_update.exchange) == 0);
  assert(symbol_.compare(market_by_price_update.symbol) == 0);
  if (ROQ_UNLIKELY(download_))
    log::info("MarketByPriceUpdate={}"_fmt, market_by_price_update);
  // update depth
  // note!
  //   market by price only gives you *changes*.
  //   you will most likely want to use the the price to look up
  //   the relative position in an order book and then modify the
  //   liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  auto depth = depth_builder_->update(market_by_price_update);
  log::trace_1("[{}:{}] depth=[{}]"_fmt, exchange_, symbol_, roq::join(depth_, ", "_sv));
  if (depth > 0 && is_ready())
    update_model();
}

void Instrument::operator()(const MarketByOrderUpdate &market_by_order_update) {
  assert(exchange_.compare(market_by_order_update.exchange) == 0);
  assert(symbol_.compare(market_by_order_update.symbol) == 0);
  if (ROQ_UNLIKELY(download_))
    log::info("MarketByOrderUpdate={}"_fmt, market_by_order_update);
  // update depth
  // note!
  //   market by order only gives you *changes*.
  //   you will most likely want to use the the price and order_id
  //   to look up the relative position in an order book and then
  //   modify the liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  auto depth = depth_builder_->update(market_by_order_update);
  log::trace_1("[{}:{}] depth=[{}]"_fmt, exchange_, symbol_, roq::join(depth_, ", "_sv));
  if (depth > 0 && is_ready())
    update_model();
}

void Instrument::update_model() {
  // one sided market?
  if (compare(depth_[0].bid_quantity, 0.0) == 0 || compare(depth_[0].ask_quantity, 0.0) == 0)
    return;
  // validate depth
  auto spread = depth_[0].ask_price - depth_[0].bid_price;
  if (ROQ_UNLIKELY(compare(spread, 0.0) <= 0))
    log::fatal(
        "[{}:{}] Probably something wrong: "
        "choice price or price inversion detected. "
        "depth=[{}]"_fmt,
        exchange_,
        symbol_,
        roq::join(depth_, ", "_sv));
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
  log::trace_1(
      "[{}:{}] model={{mid_price={}, avg_price={}}}"_fmt,
      exchange_,
      symbol_,
      mid_price_,
      avg_price_);
}

void Instrument::check_ready() {
  auto before = ready_;
  ready_ = connection_status_ == ConnectionStatus::CONNECTED && !download_ &&
           compare(tick_size_, 0.0) > 0 && compare(min_trade_vol_, 0.0) > 0 &&
           compare(multiplier_, 0.0) > 0 && trading_status_ == TradingStatus::OPEN &&
           stream_status_ == GatewayStatus::READY;
  if (ROQ_UNLIKELY(ready_ != before))
    log::info("[{}:{}] ready={}"_fmt, exchange_, symbol_, ready_);
}

void Instrument::reset() {
  connection_status_ = {};
  download_ = false;
  tick_size_ = NaN;
  min_trade_vol_ = NaN;
  trading_status_ = {};
  stream_status_ = {};
  depth_builder_->reset();
  mid_price_ = NaN;
  avg_price_ = NaN;
  ready_ = false;
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
