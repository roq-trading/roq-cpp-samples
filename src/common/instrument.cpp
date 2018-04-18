/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/instrument.h"

#include <roq/logging.h>
#include <roq/stream.h>

namespace examples {
namespace common {

Instrument::Instrument(
    size_t index,
    Gateway& gateway,
    const std::string& exchange,
    const std::string& symbol,
    double risk_limit,
    double long_position,
    double short_position,
    double tick_size)
    : _index(index),
      _gateway(gateway),
      _exchange(exchange),
      _symbol(symbol),
      _risk_limit(risk_limit),
      _tradeable(_risk_limit != 0.0),
      _market_data {
        .index = _index,
        .exchange = _exchange.c_str(),
        .symbol = _symbol.c_str()
      },
      _tick_size(tick_size),
      _long_position(long_position),
      _short_position(short_position) {
}

void Instrument::reset() {
  _market_open = false;
  _long_position.reset();
  _short_position.reset();
  _live_orders.clear();
}

bool Instrument::is_ready() const {
  return _gateway.is_ready() && _market_open;
}

double Instrument::get_long_position(PositionType type) const {
  return _long_position.get(type);
}

double Instrument::get_short_position(PositionType type) const {
  return _short_position.get(type);
}

double Instrument::get_net_position(PositionType type) const {
  return _long_position.get(type) - _short_position.get(type);
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
  switch (position_update.side) {
    case roq::Side::Buy:
      _long_position.on(position_update);
      break;
    case roq::Side::Sell:
      _short_position.on(position_update);
      break;
    default:
      LOG(FATAL) << "Unexpected";
  }
}

// Note! Order updates may be sent live or during the download phase.
// During the download phase we will receive everything previously
// sent to the gateway. Thus, by managing reconnection and download
// events, we're able to recover the state at which we left off if
// either the gateway restarts (or reconnects) or the client for
// whatever reason has to be restarted.
void Instrument::on(const roq::OrderUpdateEvent& event) {
  const auto& order_update = event.order_update;
  switch (order_update.order_status) {
    // normal
    case roq::OrderStatus::Sent:
    case roq::OrderStatus::Accepted:
    case roq::OrderStatus::Pending:
    case roq::OrderStatus::Working:
    case roq::OrderStatus::Completed:
      break;
    // missed?
    case roq::OrderStatus::Canceled:
      LOG(WARNING) << "*** ORDER WAS CANCELED ***";
      break;
    default:
      LOG(WARNING) << "Unhandled order_status=" << order_update.order_status;
      return;
  }
  // determine if the intention was to open or close
  auto open = _gateway.parse_open_close(order_update.order_template);
  // computed fill quantity (based on traded quantity vs previous)
  auto fill_quantity = _gateway.get_fill_quantity(order_update);
  // account
  auto account = order_update.account;
  // update positions for new activity
  switch (order_update.side) {
    case roq::Side::Buy: {
      if (open)
        _long_position.open(order_update.order_id, fill_quantity);
      else
        _short_position.close(order_update.order_id, fill_quantity);
      break;
    }
    case roq::Side::Sell: {
      if (open)
        _short_position.open(order_update.order_id, fill_quantity);
      else
        _long_position.close(order_update.order_id, fill_quantity);
      break;
    }
    default: {
      LOG(FATAL) << "Unexpected";
    }
  }
  if (event.message_info.from_cache == false) {
    auto position = _long_position.get(PositionType::Current) -
        _short_position.get(PositionType::Current);
    LOG(INFO) << "position=" << position;
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
      order_template,
      *this);
  _live_orders.insert(order_id);
  return order_id;
}

void Instrument::modify_order(
    uint32_t order_id,
    double quantity_change,
    double limit_price) {
  LOG_IF(FATAL, can_trade() == false) << "Unexpected";
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (is_ready() == false)
    throw roq::NotReady();
  _gateway.modify_order(
      order_id,
      quantity_change,
      limit_price,
      *this);
}

void Instrument::cancel_order(uint32_t order_id) {
  LOG_IF(FATAL, can_trade() == false) << "Unexpected";
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (is_ready() == false)
    throw roq::NotReady();
  _gateway.cancel_order(
      order_id,
      *this);
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
  return stream << "{"
    "index=" << _index << ", "
    "exchange=" << _exchange << ", "
    "symbol=" << _symbol << ", "
    "tick_size=" << _tick_size << ", "
    "market_open=" << (_market_open ? "true" : "false") << ", "
    "long_position=" << _long_position << ", "
    "short_position=" << _short_position <<
    // drop market data (not really state management)
    "}";
}

}  // namespace common
}  // namespace examples
