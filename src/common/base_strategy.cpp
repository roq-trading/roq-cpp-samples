/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/base_strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

namespace examples {
namespace common {

namespace {
std::vector<Instrument> create(Gateway& gateway, const Config& config) {
  std::vector<Instrument> result;
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    result.emplace_back(
      i,
      gateway,
      instrument.exchange,
      instrument.instrument,
      instrument.risk_limit,
      instrument.long_position,
      instrument.short_position,
      instrument.tick_size);
  }
  return result;
}
std::unordered_map<std::string, Instrument *> create(
    std::vector<Instrument>& instruments) {
  std::unordered_map<std::string, Instrument *> result;
  for (auto& instrument : instruments)
    result.emplace(instrument.get_instrument(), &instrument);
  return result;
}
}  // namespace

BaseStrategy::BaseStrategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config)
    : _gateway(dispatcher, gateway),
      _instruments(create(_gateway, config)),
      _lookup(create(_instruments)) {
}

// event handlers

// download

void BaseStrategy::on(const roq::DownloadBeginEvent& event) {
  _gateway.on(event);
  for (auto& instrument : _instruments)
    instrument.reset();
  _instruments_ready = false;
}

void BaseStrategy::on(const roq::DownloadEndEvent& event) {
  _gateway.on(event);
  for (const auto& instrument : _instruments)
    LOG(INFO) << instrument;
}

// batch

void BaseStrategy::on(const roq::BatchBeginEvent&) {
  LOG_IF(FATAL, _dirty.empty() == false) << "Unexpected";
}

void BaseStrategy::on(const roq::BatchEndEvent&) {
  for (auto instrument : _dirty)
    update(instrument->get_market_data());
  _dirty.clear();
}

// market data or order manager

void BaseStrategy::on(const roq::GatewayStatusEvent& event) {
  _gateway.on(event);
}

// order manager

void BaseStrategy::on(const roq::ReferenceDataEvent& event) {
  apply(
      event.reference_data.exchange,
      event.reference_data.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

void BaseStrategy::on(const roq::MarketStatusEvent& event) {
  if (apply(
      event.market_status.exchange,
      event.market_status.instrument,
      [&event](Instrument& instrument) {instrument.on(event); })) {
    // are *all* tradeable instruments ready for trading?
    bool ready = true;
    for (const auto& instrument : _instruments)
      if (instrument.can_trade())
        ready = instrument.is_ready() ? ready : false;
    _instruments_ready = ready;
  }
}

// Note! Position updates are only sent during the download phase.
void BaseStrategy::on(const roq::PositionUpdateEvent& event) {
  apply(
      event.position_update.exchange,
      event.position_update.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

// Note! Order updates may be sent live or during the download phase.
// During the download phase we will receive everything previously
// sent to the gateway. Thus, by managing reconnection and download
// events, we're able to recover the state at which we left off if
// either the gateway restarts (or reconnects) or the client for
// whatever reason has to be restarted.
void BaseStrategy::on(const roq::OrderUpdateEvent& event) {
  _gateway.on(event);  // track max order id
  apply(
      event.order_update.exchange,
      event.order_update.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

// request-response

void BaseStrategy::on(const roq::CreateOrderAckEvent& event) {
  apply(
      event.create_order_ack.exchange,
      event.create_order_ack.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

void BaseStrategy::on(const roq::ModifyOrderAckEvent& event) {
  apply(
      event.modify_order_ack.exchange,
      event.modify_order_ack.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

void BaseStrategy::on(const roq::CancelOrderAckEvent& event) {
  apply(
      event.cancel_order_ack.exchange,
      event.cancel_order_ack.instrument,
      [&event](Instrument& instrument) { instrument.on(event); });
}

// market data

void BaseStrategy::on(const roq::MarketByPriceEvent& event) {
  apply(
      event.market_by_price.exchange,
      event.market_by_price.instrument,
      [this, &event](Instrument& instrument) {
        instrument.on(event);
        _dirty.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::TradeSummaryEvent& event) {
  apply(
      event.trade_summary.exchange,
      event.trade_summary.instrument,
      [this, &event](Instrument& instrument) {
        instrument.on(event);
        _dirty.insert(&instrument);
      });
}

bool BaseStrategy::is_ready() const {
  return _gateway.is_ready() && _instruments_ready;
}

// general utilities

bool BaseStrategy::apply(
    const std::string& exchange,
    const std::string& instrument,
    std::function<void(Instrument&)> function) {
  auto iter = _lookup.find(instrument);
  if (iter != _lookup.end())
    function(*(*iter).second);
}

}  // namespace common
}  // namespace examples
