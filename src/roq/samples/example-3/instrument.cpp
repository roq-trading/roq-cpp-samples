/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/instrument.h"

#include <algorithm>

#include "roq/client.h"
#include "roq/logging.h"

#include "roq/utils/compare.h"
#include "roq/utils/mask.h"
#include "roq/utils/update.h"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_3 {

Instrument::Instrument(
    const std::string_view &exchange, const std::string_view &symbol, const std::string_view &account)
    : exchange_(exchange), symbol_(symbol), account_(account),
      market_by_price_(client::MarketByPriceFactory::create(exchange, symbol)) {
}

double Instrument::position() const {
  return (std::isnan(long_position_) ? 0.0 : long_position_) - (std::isnan(short_position_) ? 0.0 : short_position_);
}

bool Instrument::can_trade(Side side) const {
  switch (side) {
    case Side::BUY:
      return utils::compare(position(), 0.0) <= 0;
    case Side::SELL:
      return utils::compare(position(), 0.0) >= 0;
    default:
      assert(false);  // why is this function being called?
      return false;
  }
}

void Instrument::operator()(const Connected &) {
  if (utils::update(connected_, true)) {
    log::info("[{}:{}] connected={}"sv, exchange_, symbol_, connected_);
    check_ready();
  }
}

void Instrument::operator()(const Disconnected &) {
  if (utils::update(connected_, false)) {
    log::info("[{}:{}] connected={}"sv, exchange_, symbol_, connected_);
    // reset all cached state - await download upon reconnection
    reset();
  }
}

void Instrument::operator()(const DownloadBegin &download_begin) {
  if (!download_begin.account.empty())  // we only care about market (not account)
    return;
  assert(!download_);
  download_ = true;
  log::info("[{}:{}] download={}"sv, exchange_, symbol_, download_);
}

void Instrument::operator()(const DownloadEnd &download_end) {
  if (!download_end.account.empty())  // we only care about market (not account)
    return;
  assert(download_);
  download_ = false;
  log::info("[{}:{}] download={}"sv, exchange_, symbol_, download_);
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const GatewayStatus &gateway_status) {
  // because the API doesn't (yet) expose Mask
  utils::Mask<SupportType> available(gateway_status.available), unavailable(gateway_status.unavailable);
  if (gateway_status.account.empty()) {
    // bit-mask of required message types
    static const utils::Mask<SupportType> required{
        SupportType::REFERENCE_DATA,
        SupportType::MARKET_STATUS,
        SupportType::MARKET_BY_PRICE,
    };
    // readiness defined by full availability of all required message types
    auto market_data = available.has_all(required) && unavailable.has_none(required);
    if (utils::update(market_data_, market_data))
      log::info("[{}:{}] market_data={}"sv, exchange_, symbol_, market_data_);
    // sometimes useful to see what is missing
    if (!market_data_) {
      auto missing = required & ~available;
      log::debug("missing={:#x}"sv, missing.get());
    }
  } else if (gateway_status.account.compare(account_) == 0) {
    // bit-mask of required message types
    static const utils::Mask<SupportType> required{
        SupportType::CREATE_ORDER,
        SupportType::CANCEL_ORDER,
        SupportType::ORDER,
        SupportType::POSITION,
    };
    // readiness defined by full availability of all required message types
    auto order_management = available.has_all(required) && unavailable.has_none(required);
    if (utils::update(order_management_, order_management))
      log::info("[{}:{}] order_management={}"sv, exchange_, symbol_, order_management_);
    // sometimes useful to see what is missing
    if (!market_data_) {
      auto missing = required & ~available;
      log::debug("missing={:#x}"sv, missing.get());
    }
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const ReferenceData &reference_data) {
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

void Instrument::operator()(const MarketStatus &market_status) {
  assert(exchange_.compare(market_status.exchange) == 0);
  assert(symbol_.compare(market_status.symbol) == 0);
  // update our cache
  if (utils::update(trading_status_, market_status.trading_status)) {
    log::info("[{}:{}] trading_status={}"sv, exchange_, symbol_, trading_status_);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketByPriceUpdate &market_by_price_update) {
  assert(exchange_.compare(market_by_price_update.exchange) == 0);
  assert(symbol_.compare(market_by_price_update.symbol) == 0);
  if (ROQ_UNLIKELY(download_))
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
  market_by_price_->extract(depth_, true);
  log::info<1>("[{}:{}] depth=[{}]"sv, exchange_, symbol_, fmt::join(depth_, ", "sv));
  validate(depth_);
}

void Instrument::operator()(const OrderUpdate &order_update) {
  // note!
  //   the assumption is that there is never more than one working
  //   order
  if (last_order_id_ != order_update.order_id) {
    last_order_id_ = order_update.order_id;
    last_traded_quantity_ = 0.0;
  }
  auto quantity = order_update.traded_quantity - last_traded_quantity_;
  last_traded_quantity_ = order_update.traded_quantity;
  switch (order_update.side) {
    case Side::BUY:
      long_position_ += quantity;
      break;
    case Side::SELL:
      short_position_ += quantity;
      break;
    default:
      assert(false);  // unexpected
  }
  log::info("[{}:{}] position={}"sv, exchange_, symbol_, position());
}

void Instrument::operator()(const PositionUpdate &position_update) {
  assert(account_.compare(position_update.account) == 0);
  log::info("[{}:{}] position_update={}"sv, exchange_, symbol_, position_update);
  if (download_) {
    // note!
    //   only update positions when downloading
    //   at run-time we're better off maintaining own positions
    //   since the position feed could be broken or very delayed
    long_position_ = position_update.long_quantity;
    short_position_ = position_update.short_quantity;
  }
}

void Instrument::check_ready() {
  auto before = ready_;
  ready_ = connected_ && !download_ && utils::compare(tick_size_, 0.0) > 0 && utils::compare(min_trade_vol_, 0.0) > 0 &&
           utils::compare(multiplier_, 0.0) > 0 && trading_status_ == TradingStatus::OPEN && market_data_ &&
           order_management_;
  if (ROQ_UNLIKELY(ready_ != before))
    log::info("[{}:{}] ready={}"sv, exchange_, symbol_, ready_);
}

void Instrument::reset() {
  connected_ = false;
  download_ = false;
  tick_size_ = NaN;
  min_trade_vol_ = NaN;
  trading_status_ = {};
  market_data_ = false;
  order_management_ = false;
  market_by_price_->clear();
  long_position_ = {};
  short_position_ = {};
  ready_ = false;
  last_order_id_ = {};
  last_traded_quantity_ = {};
}

void Instrument::validate(const Depth &depth) {
  if (utils::compare(depth[0].bid_quantity, 0.0) <= 0 || utils::compare(depth[0].ask_quantity, 0.0) <= 0)
    return;
  auto spread = depth[0].ask_price - depth[0].bid_price;
  if (ROQ_UNLIKELY(utils::compare(spread, 0.0) <= 0))
    log::fatal(
        "[{}:{}] Probably something wrong: "
        "choice price or price inversion detected. "
        "depth=[{}]"sv,
        exchange_,
        symbol_,
        fmt::join(depth, ", "sv));
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
