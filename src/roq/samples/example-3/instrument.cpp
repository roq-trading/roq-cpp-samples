/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/instrument.h"

#include <algorithm>

#include "roq/logging.h"

#include "roq/client.h"

#include "roq/samples/example-3/utilities.h"

using namespace std::literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_3 {

constexpr double TOLERANCE = 1.0e-10;

Instrument::Instrument(
    const std::string_view &exchange,
    const std::string_view &symbol,
    const std::string_view &account)
    : exchange_(exchange), symbol_(symbol), account_(account),
      depth_builder_(client::DepthBuilderFactory::create(symbol, depth_)) {
}

double Instrument::position() const {
  return (std::isnan(long_position_) ? double{0.0} : long_position_) -
         (std::isnan(short_position_) ? double{0.0} : short_position_);
}

bool Instrument::can_trade(Side side) const {
  switch (side) {
    case Side::BUY:
      return position() <= TOLERANCE;
    case Side::SELL:
      return position() >= -TOLERANCE;
    default:
      assert(false);  // unexpected / why call this function at all?
      return false;
  }
}

void Instrument::operator()(const Connection &connection) {
  if (update(connection_status_, connection.status)) {
    LOG(INFO)
    (R"([{}:{}] connection_status={})"sv, exchange_, symbol_, connection_status_);
    checkready_();
  }
  switch (connection_status_) {
    case ConnectionStatus::UNDEFINED:
      LOG(FATAL)("Unexpected"sv);
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
  if (download_begin.account.empty() == false)
    return;
  assert(download_ == false);
  download_ = true;
  LOG(INFO)(R"([{}:{}] download={})"sv, exchange_, symbol_, download_);
}

void Instrument::operator()(const DownloadEnd &download_end) {
  if (download_end.account.empty() == false)
    return;
  assert(download_ == true);
  download_ = false;
  LOG(INFO)(R"([{}:{}] download={})"sv, exchange_, symbol_, download_);
  // update the ready flag
  checkready_();
}

void Instrument::operator()(const MarketDataStatus &market_data_status) {
  // update our cache
  if (update(market_data_status_, market_data_status.status)) {
    LOG(INFO)
    (R"([{}:{}] market_data_status={})"sv, exchange_, symbol_, market_data_status_);
  }
  // update the ready flag
  checkready_();
}

void Instrument::operator()(const OrderManagerStatus &order_manager_status) {
  assert(account_.compare(order_manager_status.account) == 0);
  // update our cache
  if (update(order_manager_status_, order_manager_status.status)) {
    LOG(INFO)
    (R"([{}:{}] order_manager_status={})"sv, exchange_, symbol_, order_manager_status_);
  }
  // update the ready flag
  checkready_();
}

void Instrument::operator()(const ReferenceData &reference_data) {
  assert(exchange_.compare(reference_data.exchange) == 0);
  assert(symbol_.compare(reference_data.symbol) == 0);
  // update the depth builder
  depth_builder_->update(reference_data);
  // update our cache
  if (update(tick_size_, reference_data.tick_size)) {
    LOG(INFO)(R"([{}:{}] tick_size={})"sv, exchange_, symbol_, tick_size_);
  }
  if (update(min_trade_vol_, reference_data.min_trade_vol)) {
    LOG(INFO)
    (R"([{}:{}] min_trade_vol={})"sv, exchange_, symbol_, min_trade_vol_);
  }
  if (update(multiplier_, reference_data.multiplier)) {
    LOG(INFO)(R"([{}:{}] multiplier={})"sv, exchange_, symbol_, multiplier_);
  }
  // update the ready flag
  checkready_();
}

void Instrument::operator()(const MarketStatus &market_status) {
  assert(exchange_.compare(market_status.exchange) == 0);
  assert(symbol_.compare(market_status.symbol) == 0);
  // update our cache
  if (update(trading_status_, market_status.trading_status)) {
    LOG(INFO)
    (R"([{}:{}] trading_status={})"sv, exchange_, symbol_, trading_status_);
  }
  // update the ready flag
  checkready_();
}

void Instrument::operator()(const MarketByPriceUpdate &market_by_price_update) {
  assert(exchange_.compare(market_by_price_update.exchange) == 0);
  assert(symbol_.compare(market_by_price_update.symbol) == 0);
  LOG_IF(INFO, download_)(R"(MarketByPriceUpdate={})"sv, market_by_price_update);
  // update depth
  // note!
  //   market by price only gives you *changes*.
  //   you will most likely want to use the the price to look up
  //   the relative position in an order book and then modify the
  //   liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  depth_builder_->update(market_by_price_update);
  VLOG(1)(R"([{}:{}] depth=[{}])"sv, exchange_, symbol_, fmt::join(depth_, ", "));
  validate(depth_);
}

void Instrument::operator()(const MarketByOrderUpdate &market_by_order_update) {
  assert(exchange_.compare(market_by_order_update.exchange) == 0);
  assert(symbol_.compare(market_by_order_update.symbol) == 0);
  LOG_IF(INFO, download_)(R"(MarketByOrderUpdate={})"sv, market_by_order_update);
  // update depth
  // note!
  //   market by order only gives you *changes*.
  //   you will most likely want to use the the price and order_id
  //   to look up the relative position in an order book and then
  //   modify the liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  /*
  depth_builder_->update(market_by_order_update);
  VLOG(1)(
      R"([{}:{}] depth=[{}])",
      exchange_,
      symbol_,
      fmt::join(depth_, ", "));
  validate(depth_);
  */
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
  LOG(INFO)(R"([{}:{}] position={})", exchange_, symbol_, position());
}

void Instrument::operator()(const PositionUpdate &position_update) {
  assert(account_.compare(position_update.account) == 0);
  LOG(INFO)
  (R"([{}:{}] position_update={})"sv, exchange_, symbol_, position_update);
  if (download_) {
    // note!
    //   only update positions when downloading
    //   at run-time we're better off maintaining own positions
    //   since the position feed could be broken or very delayed
    switch (position_update.side) {
      case Side::UNDEFINED: {
        long_position_ = std::max(0.0, position_update.position);
        short_position_ = std::max(0.0, -position_update.position);
        break;
      }
      case Side::BUY: {
        long_position_ = position_update.position;
        break;
      }
      case Side::SELL: {
        short_position_ = position_update.position;
        break;
      }
      default: {
        LOG(WARNING)(R"(Unexpected side={})"sv, position_update.side);
      }
    }
  }
}

void Instrument::checkready_() {
  auto before = ready_;
  ready_ = connection_status_ == ConnectionStatus::CONNECTED && download_ == false &&
           tick_size_ > TOLERANCE && min_trade_vol_ > TOLERANCE && multiplier_ > TOLERANCE &&
           trading_status_ == TradingStatus::OPEN && market_data_status_ == GatewayStatus::READY &&
           order_manager_status_ == GatewayStatus::READY;
  LOG_IF(INFO, ready_ != before)
  (R"([{}:{}] ready={})"sv, exchange_, symbol_, ready_);
}

void Instrument::reset() {
  connection_status_ = ConnectionStatus::DISCONNECTED;
  download_ = false;
  tick_size_ = std::numeric_limits<double>::quiet_NaN();
  min_trade_vol_ = std::numeric_limits<double>::quiet_NaN();
  trading_status_ = TradingStatus::UNDEFINED;
  market_data_status_ = GatewayStatus::DISCONNECTED;
  order_manager_status_ = GatewayStatus::DISCONNECTED;
  depth_builder_->reset();
  long_position_ = 0.0;
  short_position_ = 0.0;
  ready_ = false;
  last_order_id_ = 0;
  last_traded_quantity_ = 0.0;
}

void Instrument::validate(const Depth &depth) {
  if (std::fabs(depth[0].bid_quantity) < TOLERANCE || std::fabs(depth[0].ask_quantity) < TOLERANCE)
    return;
  auto spread = depth[0].ask_price - depth[0].bid_price;
  LOG_IF(FATAL, spread < TOLERANCE)
  (R"([{}:{}] Probably something wrong: )"
   R"(choice or inversion detected. depth=[{}])"sv,
   exchange_,
   symbol_,
   fmt::join(depth, ", "));
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
