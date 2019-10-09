/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/common/instrument.h"

#include <cmath>
#include <limits>
#include <utility>

#include "roq/logging.h"
#include "roq/stream.h"

#include "roq/samples/common/account.h"

#define PREFIX "[" << _symbol << "] "

namespace roq {
namespace samples {
namespace common {

namespace {
constexpr double TOLERANCE = 1.0e-8;
}  // namespace

Instrument::Instrument(
    size_t index,
    const std::string& exchange,
    const std::string& symbol,
    const double net_limit,
    const double tick_size,
    const double multiplier,
    std::vector<Position *>&& positions)
    : _index(index),
      _exchange(exchange),
      _symbol(symbol),
      _net_limit(net_limit),
      _market_data {
        .index = _index,
        .exchange = _exchange.c_str(),
        .symbol = _symbol.c_str(),
        .tick_size = tick_size,
        .multiplier = multiplier,
        .depth = {},
        .price = std::numeric_limits<double>::quiet_NaN(),
        .volume = std::numeric_limits<double>::quiet_NaN(),
        .turnover = std::numeric_limits<double>::quiet_NaN(),
        .side = Side::UNDEFINED,
        .exchange_time = {}
      },
      _depth_builder(
          client::DepthBuilder::create(
              _market_data.depth,
              std::size(_market_data.depth))),
      _tradeable(index == 0),
      _positions(std::move(positions)) {
}

double Instrument::get_position() const {
  double result = 0.0;
  for (const auto& iter : _positions)
    result += iter->get_net();
  return result;
}

void Instrument::on(const MarketDataStatus& market_data_status) {
  auto market_data_ready =
    market_data_status.status == GatewayStatus::READY;
  if (_market_data_ready != market_data_ready) {
    _market_data_ready = market_data_ready;
    LOG(INFO) << PREFIX "market_data_ready=" <<
      (_market_data_ready ? "true" : "false");
  }
}

void Instrument::on(const SessionStatistics&) {
}

void Instrument::on(const DailyStatistics&) {
}

void Instrument::on(const MarketByPrice& market_by_price) {
  _depth_builder->update(market_by_price, true);
  _market_data.exchange_time = market_by_price.exchange_time_utc;
}

void Instrument::on(const TradeSummary& trade_summary) {
  /* FIXME(thraneh): this is broken
  _market_data.price = trade_summary.price;
  _market_data.volume = trade_summary.volume;
  _market_data.turnover = trade_summary.turnover;
  _market_data.side = trade_summary.side;
  _market_data.exchange_time = trade_summary.exchange_time_utc;
  */
}

void Instrument::on(const ReferenceData& reference_data) {
  auto tick_size = reference_data.tick_size;
  if (_market_data.tick_size != tick_size && tick_size != 0.0) {
    _market_data.tick_size = tick_size;
    LOG(INFO) << PREFIX "tick_size=" << _market_data.tick_size;
  }
  auto multiplier = reference_data.multiplier;
  if (_market_data.multiplier != multiplier && multiplier != 0.0) {
    _market_data.multiplier = multiplier;
    LOG(INFO) << PREFIX "multiplier=" << _market_data.multiplier;
  }
}

void Instrument::on(const MarketStatus& market_status) {
  auto market_open = market_status.trading_status == TradingStatus::OPEN;
  if (_market_open != market_open) {
    _market_open = market_open;
    LOG(INFO) << PREFIX "market_open=" << (_market_open ? "true" : "false");
  }
}

void Instrument::create_ioc(Side side, double quantity, double price) {
  // FIXME(thraneh): this can be done a lot more efficiently...
  // first check net limit
  auto net = get_position();
  auto delta = (side == Side::BUY ? 1.0 : -1.0) * quantity;
  if (std::fabs(net + delta) > (_net_limit + TOLERANCE)) {
    VLOG(1) << "Unable to trade. "
      "Net position limit has been exhausted {"
      "side=" << side << ", "
      "quantity=" << quantity << ", "
      "price=" << price <<
      "}";
    return;
  }
  // then close anything left open from yesterday
  for (auto& position : _positions) {
    auto position_effect = position->get_effect(side, quantity);
    if (position_effect == PositionEffect::CLOSE) {
      position->get_account().create_order(
          _exchange,
          _symbol,
          side,
          quantity,
          price,
          TimeInForce::IOC,
          position_effect);
      return;
    }
  }
  // finally open until position limit is hit
  for (auto& position : _positions) {
    auto position_effect = position->get_effect(side, quantity);
    if (position_effect == PositionEffect::OPEN) {
      position->get_account().create_order(
          _exchange,
          _symbol,
          side,
          quantity,
          price,
          TimeInForce::IOC,
          position_effect);
      return;
    }
  }
  // warn in case account long/short limit is being tested and fails
  LOG(WARNING) << "Unable to trade. "
    "All position limits have been exhausted {"
    "side=" << side << ", "
    "quantity=" << quantity << ", "
    "price=" << price <<
    "}";
}

}  // namespace common
}  // namespace samples
}  // namespace roq
