/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "common/instrument.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include "common/account.h"

#define PREFIX "[" << _symbol << "] "
#define TOLERANCE 1.0e-8

namespace examples {
namespace common {

Instrument::Instrument(
    size_t index,
    const std::string& exchange,
    const std::string& symbol,
    double tick_size,
    double multiplier,
    double net_limit,
    std::vector<std::shared_ptr<Position> >&& positions)
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
      },
      _positions(std::move(positions)),
      _tradeable(_positions.empty() == false) {
}

bool Instrument::is_ready() const {
  return _market_data_ready && _market_open;
}

double Instrument::get_position() const {
  double result = 0.0;
  for (const auto& iter : _positions)
    result += iter->get_net();
  return result;
}

void Instrument::on(const roq::MarketDataStatus& market_data_status) {
  auto market_data_ready =
      market_data_status.status == roq::GatewayStatus::Ready;
  if (_market_data_ready != market_data_ready) {
    _market_data_ready = market_data_ready;
    LOG(INFO) << PREFIX "market_data_ready=" <<
      (_market_data_ready ? "true" : "false");
  }
}

void Instrument::on(const roq::SessionStatistics& session_statistics) {
}

void Instrument::on(const roq::DailyStatistics& daily_statistics) {
}

void Instrument::on(const roq::MarketByPrice& market_by_price) {
  std::memcpy(
      _market_data.depth,
      market_by_price.depth,
      sizeof(market_by_price.depth));
  _market_data.exchange_time = market_by_price.exchange_time;
  _market_data.channel = market_by_price.channel;
}

void Instrument::on(const roq::TradeSummary& trade_summary) {
  _market_data.price = trade_summary.price;
  _market_data.volume = trade_summary.volume;
  _market_data.turnover = trade_summary.turnover;
  _market_data.side = trade_summary.side;
  _market_data.exchange_time = trade_summary.exchange_time;
  _market_data.channel = trade_summary.channel;
}

void Instrument::on(const roq::ReferenceData& reference_data) {
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

void Instrument::on(const roq::MarketStatus& market_status) {
  auto market_open = market_status.trading_status == roq::TradingStatus::Open;
  if (_market_open != market_open) {
    _market_open = market_open;
    LOG(INFO) << PREFIX "market_open=" << (_market_open ? "true" : "false");
  }
}

void Instrument::buy_ioc(double quantity, double price) {
  create_ioc(roq::Side::Buy, quantity, price);
}

void Instrument::sell_ioc(double quantity, double price) {
  create_ioc(roq::Side::Sell, quantity, price);
}

void Instrument::create_ioc(
    roq::Side side,
    double quantity,
    double price) {
  // FIXME(thraneh): this can be done a lot more efficiently...
  // first check net limit
  auto net = get_position();
  auto delta = (side == roq::Side::Buy ? 1.0 : -1.0) * quantity;
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
    if (position_effect == roq::PositionEffect::Close) {
      position->get_account().create_order(
          _exchange,
          _symbol,
          side,
          quantity,
          price,
          roq::TimeInForce::IOC,
          position_effect);
      return;
    }
  }
  // finally open until position limit is hit
  for (auto& position : _positions) {
    auto position_effect = position->get_effect(side, quantity);
    if (position_effect == roq::PositionEffect::Open) {
      position->get_account().create_order(
          _exchange,
          _symbol,
          side,
          quantity,
          price,
          roq::TimeInForce::IOC,
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

std::ostream& Instrument::write(std::ostream& stream) const {
  return stream << "{"
    "index=" << _index << ", "
    "exchange=" << _exchange << ", "
    "symbol=" << _symbol << ", "
    "net_limit=" << _net_limit << ", "
    "tick_size=" << _market_data.tick_size << ", "
    "multiplier=" << _market_data.multiplier << ", "
    "market_open=" << (_market_open ? "true" : "false") <<
    "}";
}

}  // namespace common
}  // namespace examples
