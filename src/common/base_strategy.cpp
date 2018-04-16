/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/base_strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

namespace examples {
namespace common {

namespace {
// constructor utilities
std::vector<Instrument> instruments(Gateway& gateway, const Config& config) {
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
std::unordered_map<std::string, Instrument *> lookup(
    std::vector<Instrument>& instruments) {
  std::unordered_map<std::string, Instrument *> result;
  for (auto& instrument : instruments)
    result.emplace(instrument.get_instrument(), &instrument);
  return result;
}
roq::Strategy::subscriptions_t subscriptions(
    const std::string& gateway,
    const std::vector<Instrument>& instruments) {
  roq::Strategy::subscriptions_t result;
  auto& tmp = result[gateway];
  for (const auto& instrument : instruments)
    tmp[instrument.get_exchange()].emplace_back(instrument.get_instrument());
  return result;
}
}  // namespace

BaseStrategy::BaseStrategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config)
    : _gateway(dispatcher, gateway),
      _instruments(instruments(_gateway, config)),
      _lookup(lookup(_instruments)),
      _subscriptions(subscriptions(gateway, _instruments)) {
}

// event handlers

// timer
void BaseStrategy::on(const roq::TimerEvent& event) {
  _gateway.on(event);
}

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
  LOG_IF(FATAL, _market_data_updated.empty() == false) << "Unexpected";
}

void BaseStrategy::on(const roq::BatchEndEvent&) {
  for (auto instrument : _market_data_updated)
    update(instrument->get_market_data());
  _market_data_updated.clear();
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
    // are *all* tradeable instruments now ready for trading?
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
  if (_gateway.is_downloading()) {
    apply(
        event.order_update.exchange,
        event.order_update.instrument,
        [this, &event](Instrument& instrument) { _gateway.on(event, &instrument); });
  } else {
    _gateway.on(event);
  }
}

// request-response

void BaseStrategy::on(const roq::CreateOrderAckEvent& event) {
  _gateway.on(event);
}

void BaseStrategy::on(const roq::ModifyOrderAckEvent& event) {
  _gateway.on(event);
}

void BaseStrategy::on(const roq::CancelOrderAckEvent& event) {
  _gateway.on(event);
}

// market data

void BaseStrategy::on(const roq::MarketByPriceEvent& event) {
  apply(
      event.market_by_price.exchange,
      event.market_by_price.instrument,
      [this, &event](Instrument& instrument) {
        instrument.on(event);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::TradeSummaryEvent& event) {
  apply(
      event.trade_summary.exchange,
      event.trade_summary.instrument,
      [this, &event](Instrument& instrument) {
        instrument.on(event);
        _market_data_updated.insert(&instrument);
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
