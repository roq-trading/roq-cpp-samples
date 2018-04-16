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
    const std::string& instrument,
    double risk_limit,
    double long_position,
    double short_position,
    double tick_size)
    : _index(index),
      _gateway(gateway),
      _exchange(exchange),
      _instrument(instrument),
      _risk_limit(risk_limit),
      _tradeable(_risk_limit != 0.0),
      _long_position(long_position),
      _short_position(short_position),
      _tick_size(tick_size),
      _market_data {
        .index = _index,
        .exchange = _exchange.c_str(),
        .instrument = _instrument.c_str()
      } {
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

// Note! Position updates are only sent during the download phase.
void Instrument::on(const roq::PositionUpdateEvent& event) {
  const auto& position_update = event.position_update;
  // initialize start of day position using yesterday's close position
  // note! this is an example-choice, we could also have configured this.
  switch (position_update.trade_direction) {
    case roq::TradeDirection::Buy: {
      auto test = _long_position.get(PositionType::StartOfDay) != 0.0;
      LOG_IF(FATAL, test) << "Unexpected";
      _long_position.set_start_of_day(position_update.position_yesterday);
      _long_position.set_reference(position_update.position);
      break;
    }
    case roq::TradeDirection::Sell: {
      auto test = _short_position.get(PositionType::StartOfDay) != 0.0;
      LOG_IF(FATAL, test) << "Unexpected";
      _short_position.set_start_of_day(position_update.position_yesterday);
      _short_position.set_reference(position_update.position);
      break;
    }
    default: {
      LOG(FATAL) << "Unexpected";
    }
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
  // download?
  auto download = event.message_info.from_cache;  // HANS -- same as download???
  // determine if the intention was to open or close
  auto open = _gateway.parse_open_close(order_update.order_template);
  // computed fill quantity (based on traded quantity vs previous)
  auto fill_quantity = _gateway.get_fill_quantity(order_update);
  // update positions for new activity
  switch (order_update.trade_direction) {
    case roq::TradeDirection::Buy: {
      _long_position.add_new_activity(fill_quantity);
      if (download == false)
        LOG(INFO) << "long_position=" << _long_position;
      break;
    }
    case roq::TradeDirection::Sell: {
      _short_position.add_new_activity(fill_quantity);
      if (download == false)
        LOG(INFO) << "short_position=" << _short_position;
      break;
    }
    default: {
      LOG(FATAL) << "Unexpected";
    }
  }
  if (download == false) {
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
  _market_data.direction = trade_summary.direction;
  _market_data.exchange_time = trade_summary.exchange_time;
  _market_data.channel = trade_summary.channel;
}

uint32_t Instrument::create_order(
    roq::TradeDirection direction,
    double quantity,
    double price,
    const std::string& order_template) {
  LOG_IF(FATAL, _tradeable == false) << "Unexpected";
  if (is_ready() == false)
    throw roq::NotReady();
  auto order_id = _gateway.create_order(
      _exchange,
      _instrument,
      direction,
      quantity,
      price,
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
    "instrument=" << _instrument << ", "
    "tick_size=" << _tick_size << ", "
    "market_open=" << (_market_open ? "true" : "false") << ", "
    "long_position=" << _long_position << ", "
    "short_position=" << _short_position <<
    // drop market data (not really state management)
    "}";
}

}  // namespace common
}  // namespace examples
