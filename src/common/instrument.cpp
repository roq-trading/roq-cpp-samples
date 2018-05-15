/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/instrument.h"

#include <roq/logging.h>
#include <roq/stream.h>

namespace examples {
namespace common {

Instrument::Instrument(
    size_t index,
    const std::string& exchange,
    const std::string& symbol,
    double risk_limit,
    double tick_size,
    double multiplier,
    std::vector<Position *>&& positions)
    : _index(index),
      _exchange(exchange),
      _symbol(symbol),
      _risk_limit(risk_limit),
      _tradeable(_positions.empty() == false),
      _market_data {
        .index = _index,
        .exchange = _exchange.c_str(),
        .symbol = _symbol.c_str(),
        .tick_size = tick_size,
        .multiplier = multiplier,
      },
      _positions(std::move(positions)) {
}

void Instrument::reset() {
  _market_open = false;
  /*
  for (auto& iter : _positions)
    iter.second.reset();
  _live_orders.clear();
  */
}

bool Instrument::is_ready() const {
  // return _gateway.is_ready() && _market_open;
  return false;  // HANS ???
}

double Instrument::get_position() const {
  double result = 0.0;
  /*
  for (const auto& iter : _positions)
    result += iter.second.get_net();
  */
  return result;
}

void Instrument::on(const roq::ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  auto tick_size = reference_data.tick_size;
  if (_market_data.tick_size != tick_size && tick_size != 0.0) {
    _market_data.tick_size = tick_size;
    LOG(INFO) << "tick_size=" << _market_data.tick_size;
  }
  auto multiplier = reference_data.multiplier;
  if (_market_data.multiplier != multiplier && multiplier != 0.0) {
    _market_data.multiplier = multiplier;
    LOG(INFO) << "multiplier=" << _market_data.multiplier;
  }
}

void Instrument::on(const roq::MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  auto market_open = market_status.trading_status == roq::TradingStatus::Open;
  if (_market_open != market_open) {
    _market_open = market_open;
    LOG(INFO) << "market_open=" << (_market_open ? "true" : "false");
  }
}

void Instrument::on(const roq::MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  std::memcpy(
      _market_data.depth,
      market_by_price.depth,
      sizeof(market_by_price.depth));
  _market_data.exchange_time = market_by_price.exchange_time;
  _market_data.channel = market_by_price.channel;
}

void Instrument::on(const roq::TradeSummaryEvent& event) {
  const auto& trade_summary = event.trade_summary;
  _market_data.price = trade_summary.price;
  _market_data.volume = trade_summary.volume;
  _market_data.turnover = trade_summary.turnover;
  _market_data.side = trade_summary.side;
  _market_data.exchange_time = trade_summary.exchange_time;
  _market_data.channel = trade_summary.channel;
}

/*
uint32_t Instrument::create_order(
    const std::string& account,
    roq::Side side,
    double quantity,
    double price,
    roq::TimeInForce time_in_force,
    roq::PositionEffect position_effect,
    const std::string& order_template) {
  LOG_IF(FATAL, _tradeable == false) << "Unexpected";
  if (is_ready() == false) {
    LOG(WARNING) << "Instrument is not in the ready state {"
    "symbol=\"" << _symbol << "\""
    "}";
    throw roq::NotReady();
  }
  auto order_id = _gateway.create_order(
      account,
      _exchange,
      _symbol,
      side,
      quantity,
      price,
      time_in_force,
      position_effect,
      order_template,
      *this);
  _live_orders.insert(order_id);
  return order_id;
}
*/

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
  /*
  for (const auto& iter : _positions) {
    auto position_effect = iter.second.get_effect(side, quantity);
    if (position_effect == roq::PositionEffect::Close) {
      create_order(
          iter.first,
          side,
          quantity,
          price,
          roq::TimeInForce::IOC,
          roq::PositionEffect::Close,
          "default");
      return;
    }
  }
  for (const auto& iter : _positions) {
    auto position_effect = iter.second.get_effect(side, quantity);
    if (position_effect == roq::PositionEffect::Open) {
      create_order(
          iter.first,
          side,
          quantity,
          price,
          roq::TimeInForce::IOC,
          roq::PositionEffect::Open,
          "default");
      return;
    }
  }
  */
  LOG(ERROR) << "Not possible to trade";  // FIXME(thraneh): use exception
}

std::ostream& Instrument::write(std::ostream& stream) const {
  return stream << "{"
    "index=" << _index << ", "
    "exchange=" << _exchange << ", "
    "symbol=" << _symbol << ", "
    "tick_size=" << _market_data.tick_size << ", "
    "multiplier=" << _market_data.multiplier << ", "
    "market_open=" << (_market_open ? "true" : "false") <<
    "}";
}

}  // namespace common
}  // namespace examples
