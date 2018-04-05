/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "order_management/base.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

namespace examples {
namespace order_management {

const char *TRADER = "Trader";  // FIXME(thraneh): introduce an enum for this!

BaseStrategy::BaseStrategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& exchange,
    const std::string& instrument,
    const std::string& gateway,
    const std::string& ioc_open,
    const std::string& ioc_close,
    const std::string& gtc_open,
    const std::string& gtc_close)
    : _dispatcher(dispatcher),
      _exchange(exchange),
      _instrument(instrument),
      _gateway(gateway),
      _ioc_open(ioc_open),
      _ioc_close(ioc_close),
      _gtc_open(gtc_open),
      _gtc_close(gtc_close) {
}

// event handlers

void BaseStrategy::on(const roq::TimerEvent&) {
  // TODO(thraneh): check time-out
}

// download

void BaseStrategy::on(const roq::DownloadBeginEvent& event) {
  // raise the download flag to block order management
  _download = true;
  LOG(INFO) << "download=" << (_download ? "true" : "false");
  // reset all variables tracking order management state
  _order_manager_ready = false;
  _market_open = false;
  _long_position = 0.0;
  _short_position = 0.0;
}

void BaseStrategy::on(const roq::DownloadEndEvent& event) {
  const auto& download_end = event.download_end;
  // the gateway keeps track of order id's (they can't be recycled)
  auto max_order_id = std::max(_max_order_id, download_end.max_order_id);
  if (_max_order_id != max_order_id) {
    _max_order_id = max_order_id;
    LOG(INFO) << "max_order_id=" << _max_order_id;
  }
  // reset the download flag to allow order management
  _download = false;
  LOG(INFO) << "download=" << (_download ? "true" : "false");
}

// either

void BaseStrategy::on(const roq::GatewayStatusEvent& event) {
  const auto& gateway_status = event.gateway_status;
  LOG(INFO) << gateway_status;
  // return early if it's not the gateway's order management status
  if (std::strcmp(TRADER, gateway_status.name) != 0)
    return;
  // ready?
  auto order_manager_ready = gateway_status.status == roq::GatewayState::Ready;
  if (_order_manager_ready != order_manager_ready) {
    _order_manager_ready = order_manager_ready;
    LOG(INFO) << "order_manager_ready=" << (_order_manager_ready ? "true" : "false");
  }
}

// order manager

void BaseStrategy::on(const roq::ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  // return early if it's not the instrument we want to trade
  if (filter(reference_data.exchange, reference_data.instrument))
    return;
  // instrument's tick size
  if (_tick_size != reference_data.tick_size) {
    _tick_size = reference_data.tick_size;
    LOG(INFO) << "tick_size=" << _tick_size;
  }
}

void BaseStrategy::on(const roq::MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  // return early if it's not the instrument we want to trade
  if (filter(market_status.exchange, market_status.instrument))
    return;
  // instrument's trading status
  auto market_open = market_status.trading_status == roq::TradingStatus::Open;
  if (_market_open != market_open) {
    _market_open = market_open;
    LOG(INFO) << "market_open=" << (_market_open ? "true" : "false");
  }
}

void BaseStrategy::on(const roq::PositionUpdateEvent& event) {
  LOG(INFO) << event;
  const auto& position_update = event.position_update;
  // return early if it's not the instrument we want to trade
  if (filter(position_update.exchange, position_update.instrument))
    return;
  // initialize using yesterday's position
  add_trade(position_update.trade_direction, position_update.position_yesterday);
}

void BaseStrategy::on(const roq::OrderUpdateEvent& event) {
  LOG(INFO) << event;
  const auto& order_update = event.order_update;
  // return early if it's not the instrument we want to trade
  if (filter(order_update.exchange, order_update.instrument))
    return;
  // determine if the intention was to open or close
  auto open = parse_open_close(order_update.order_template);
  // compute fill quantity (and keep track of total traded quantity)
  auto& previous = _order_traded_quantity[order_update.opaque];
  auto fill_quantity = std::max(0.0, order_update.traded_quantity - previous);
  previous = order_update.traded_quantity;
  // ... question: what will we need this for?
  add_trade(order_update.trade_direction, fill_quantity);
}

// request-response

void BaseStrategy::on(const roq::CreateOrderAckEvent& event) {
  LOG(INFO) << event;
  const auto& create_order_ack = event.create_order_ack;
  LOG_IF(FATAL, _download) << "Unexpected";
}

void BaseStrategy::on(const roq::ModifyOrderAckEvent& event) {
  LOG(INFO) << event;
  const auto& modify_order_ack = event.modify_order_ack;
  LOG_IF(FATAL, _download) << "Unexpected";
}

void BaseStrategy::on(const roq::CancelOrderAckEvent& event) {
  LOG(INFO) << event;
  const auto& cancel_order_ack = event.cancel_order_ack;
  LOG_IF(FATAL, _download) << "Unexpected";
}

// market data

void BaseStrategy::on(const roq::MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  // return early if it's not the instrument we want to trade
  if (filter(market_by_price.exchange, market_by_price.instrument))
    return;
  // update trading strategy and see if it's time to trade
  update_signal(market_by_price);
  try_trade();
}

// create order helpers

uint32_t BaseStrategy::buy_ioc_open(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Buy, quantity, price, _ioc_open);
}

uint32_t BaseStrategy::sell_ioc_open(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Sell, quantity, price, _ioc_open);
}

uint32_t BaseStrategy::buy_ioc_close(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Buy, quantity, price, _ioc_close);
}

uint32_t BaseStrategy::sell_ioc_close(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Sell, quantity, price, _ioc_close);
}

uint32_t BaseStrategy::buy_gtc_open(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Buy, quantity, price, _gtc_open);
}

uint32_t BaseStrategy::sell_gtc_open(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Sell, quantity, price, _gtc_open);
}

uint32_t BaseStrategy::buy_gtc_close(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Buy, quantity, price, _gtc_close);
}

uint32_t BaseStrategy::sell_gtc_close(const double quantity, const double price) {
  return create_order(roq::TradeDirection::Sell, quantity, price, _gtc_close);
}

// Generic function to create an order.
uint32_t BaseStrategy::create_order(
    const roq::TradeDirection direction,
    const double quantity,
    const double price,
    const std::string& order_template) {
  auto order_id = ++_max_order_id;
  roq::CreateOrder create_order {
    .opaque         = order_id,
    .order_template = order_template.c_str(),
    .exchange       = _exchange.c_str(),
    .instrument     = _instrument.c_str(),
    .direction      = direction,
    .quantity       = quantity,
    .limit_price    = price,
  };
  _dispatcher.send(create_order, _gateway.c_str());
  return order_id;
}

// position management

// Update current position.
void BaseStrategy::add_trade(roq::TradeDirection direction, double quantity) {
  switch (direction) {
    case roq::TradeDirection::Buy: {
      _long_position += quantity;
      LOG(INFO) << "long_position=" << _long_position;
      break;
    }
    case roq::TradeDirection::Sell: {
      _short_position += quantity;
      LOG(INFO) << "short_position=" << _short_position;
      break;
    }
    default: {
      LOG(FATAL) << "Unexpected";
    }
  }
}

// general utilities

// Filter update?
// Returns true if the update should be filtered (excluded).
bool BaseStrategy::filter(const char *exchange, const char *instrument) {
  return _instrument.compare(instrument) != 0 || _exchange.compare(exchange) != 0;
}

// Ready to trade?
// Verifies all variables relating to state management.
// Returns true if it's possible to trade.
bool BaseStrategy::is_ready() const {
  return !_download && _order_manager_ready && _market_open;
}

// There are no specific client fields to indicate the desire to open or
// close a position. Those extra fields are only visible to the gateway.
// However, the client can use the name of an order template (as known
// by the gateway) to communicate open or close. The name of the template
// is included with every order update.
// Returns true if the order template is known as a "open".
// Returns false if the order template is known as a "close".
// Terminate program execution if the order template is unknown.
bool BaseStrategy::parse_open_close(const char *order_template) {
  if (_ioc_open.compare(order_template) == 0 || _gtc_open.compare(order_template) == 0)
    return true;
  if (_ioc_close.compare(order_template) == 0 || _gtc_close.compare(order_template) == 0)
    return false;
  LOG(FATAL) << "Unable to determine open/close from order_template=\"" << order_template << "\"";
}

}  // namespace order_management
}  // namespace examples
