/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/instrument.h"

#include <roq/logging.h>
#include <roq/stream.h>

namespace examples {
namespace common {

namespace {
std::unordered_map<std::string, Position> create_positions(
    const std::map<std::string, std::pair<double, double> >& accounts) {
  std::unordered_map<std::string, Position> result;
  for (const auto& iter : accounts)
    result.emplace(iter.first, Position(
        true,  // HANS
        iter.second.first,
        iter.second.second));
  return result;
}
}  // namespace

Instrument::Instrument(
    size_t index,
    Gateway& gateway,
    const std::string& exchange,
    const std::string& symbol,
    const std::map<std::string, std::pair<double, double> >& accounts,
    double risk_limit,
    double tick_size)
    : _index(index),
      _gateway(gateway),
      _exchange(exchange),
      _symbol(symbol),
      _positions(create_positions(accounts)),
      _risk_limit(risk_limit),
      _tradeable(_positions.empty() == false),
      _market_data {
        .index = _index,
        .exchange = _exchange.c_str(),
        .symbol = _symbol.c_str()
      },
      _tick_size(tick_size) {
}

void Instrument::reset() {
  _market_open = false;
  for (auto& iter : _positions)
    iter.second.reset();
  _live_orders.clear();
}

bool Instrument::is_ready() const {
  return _gateway.is_ready() && _market_open;
}

double Instrument::get_position() const {
  double result = 0;
  for (const auto& iter : _positions)
    result += iter.second.get_net();
  return result;
}

void Instrument::on(const roq::ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  auto tick_size = reference_data.tick_size;
  if (_tick_size != tick_size && tick_size != 0.0) {
    _tick_size = tick_size;
    LOG(INFO) << "tick_size=" << _tick_size;
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

void Instrument::on(const roq::PositionUpdateEvent& event) {
  const auto& position_update = event.position_update;
  auto account = position_update.account;
  auto iter = _positions.find(account);
  if (iter != _positions.end())
    (*iter).second.on(position_update);
  else
    LOG(WARNING) << "Received unknown account for "
      "position_update=" << position_update;
}

// Note! Order updates may be sent live or during the download phase.
// During the download phase we will receive everything previously
// sent to the gateway. Thus, by managing reconnection and download
// events, we're able to recover the state at which we left off if
// either the gateway restarts (or reconnects) or the client for
// whatever reason has to be restarted.
void Instrument::on(const roq::OrderUpdateEvent& event) {
  const auto& order_update = event.order_update;
  // TODO(thraneh): we need to record proper statistics here
  switch (order_update.order_status) {
    // normal
    case roq::OrderStatus::Sent:
    case roq::OrderStatus::Accepted:
    case roq::OrderStatus::Pending:
    case roq::OrderStatus::Working:
    case roq::OrderStatus::Completed:
      break;
    // missed
    case roq::OrderStatus::Canceled:
      break;
    default:
      LOG(WARNING) << "Received unknown or undefined order_status for "
        "order_update=" << order_update;
      return;
  }
  // update position
  auto iter = _positions.find(order_update.account);
  if (iter != _positions.end()) {
    auto& position = (*iter).second;
    position.on(order_update);
    if (!_gateway.is_downloading())
      LOG(INFO) << "Position {"
        "account=\"" << order_update.account << "\", "
        "position=" << position.get_net() <<
        "}";
  }
}

// note! trade updates *normally* arrives *after* order updates
void Instrument::on(const roq::TradeUpdateEvent& event) {
  const auto& trade_update = event.trade_update;
  // update position
  auto iter = _positions.find(trade_update.account);
  if (iter != _positions.end()) {
    auto& position = (*iter).second;
    position.on(trade_update);
    /*
    if (!_gateway.is_downloading())
      LOG(INFO) << position;
    */
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

uint32_t Instrument::create_order(
    const std::string& account,
    roq::Side side,
    double quantity,
    double price,
    roq::TimeInForce time_in_force,
    roq::PositionEffect position_effect,
    const std::string& order_template) {
  LOG_IF(FATAL, _tradeable == false) << "Unexpected";
  if (is_ready() == false)
    throw roq::NotReady();
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
  LOG(ERROR) << "Not possible to trade";  // FIXME(thraneh): use exception
}

void Instrument::modify_order(
    const std::string& account,
    uint32_t order_id,
    double quantity_change,
    double limit_price) {
  LOG_IF(FATAL, can_trade() == false) << "Unexpected";
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (is_ready() == false)
    throw roq::NotReady();
  _gateway.modify_order(
      account,
      order_id,
      quantity_change,
      limit_price);
}

void Instrument::cancel_order(
    const std::string& account,
    uint32_t order_id) {
  LOG_IF(FATAL, can_trade() == false) << "Unexpected";
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (is_ready() == false)
    throw roq::NotReady();
  _gateway.cancel_order(account, order_id);
}

void Instrument::on(const roq::CreateOrderAckEvent& event) {
  const auto& create_order_ack = event.create_order_ack;
  LOG_IF(WARNING, create_order_ack.failure) <<
      "create_order_ack=" << create_order_ack;
}

void Instrument::on(const roq::ModifyOrderAckEvent& event) {
  const auto& modify_order_ack = event.modify_order_ack;
  LOG_IF(WARNING, modify_order_ack.failure) <<
      "modify_order_ack=" << modify_order_ack;
}

void Instrument::on(const roq::CancelOrderAckEvent& event) {
  const auto& cancel_order_ack = event.cancel_order_ack;
  LOG_IF(WARNING, cancel_order_ack.failure) <<
      "cancel_order_ack=" << cancel_order_ack;
}

bool Instrument::is_order_live(uint32_t order_id) const {
  return _live_orders.find(order_id) != _live_orders.end();
}

std::ostream& Instrument::write(std::ostream& stream) const {
  stream << "{"
    "index=" << _index << ", "
    "exchange=" << _exchange << ", "
    "symbol=" << _symbol << ", "
    "tick_size=" << _tick_size << ", "
    "market_open=" << (_market_open ? "true" : "false") << ", "
    "positions={";
  bool first = true;
  for (const auto& iter : _positions) {
    if (first == false)
      stream << ", ";
    first = false;
    stream << "\"" << iter.first << "\"=" << iter.second;
  }
  return stream <<"}";
}

}  // namespace common
}  // namespace examples
