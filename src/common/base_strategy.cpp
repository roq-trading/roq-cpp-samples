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
      instrument.symbol,
      instrument.accounts,
      instrument.risk_limit,
      instrument.tick_size);
  }
  return result;
}
std::unordered_map<std::string, Instrument *> lookup(
    std::vector<Instrument>& instruments) {
  std::unordered_map<std::string, Instrument *> result;
  for (auto& instrument : instruments)
    result.emplace(instrument.get_symbol(), &instrument);
  return result;
}
roq::Strategy::subscriptions_t subscriptions(
    const std::string& gateway,
    const std::vector<Instrument>& instruments) {
  roq::Strategy::subscriptions_t result;
  auto& tmp = result[gateway];
  for (const auto& instrument : instruments)
    tmp[instrument.get_exchange()].emplace_back(instrument.get_symbol());
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

namespace {
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<Instrument>& instruments) {
  stream << "[";
  bool first = true;
  for (const auto& instrument : instruments) {
    if (!first)
      stream << ", ";
    first = false;
    stream << instrument;
  }
  return stream << "]";
}
}  // namespace

void BaseStrategy::on(const roq::DownloadEndEvent& event) {
  _gateway.on(event);
  LOG(INFO) << "instruments=" << _instruments;
}

// batch

void BaseStrategy::on(const roq::BatchBeginEvent&) {
  // internal check that end of batch has reset correctly
  LOG_IF(FATAL, !_market_data_updated.empty()) << "Unexpected";
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

void BaseStrategy::on(const roq::AccountStatusEvent& event) {
  // HANS -- what ???
}

void BaseStrategy::on(const roq::ReferenceDataEvent& event) {
  apply(
      event.reference_data.exchange,
      event.reference_data.symbol,
      [&event](Instrument& instrument) { instrument.on(event); });
}

void BaseStrategy::on(const roq::MarketStatusEvent& event) {
  if (apply(
      event.market_status.exchange,
      event.market_status.symbol,
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
  if (!apply(
      event.position_update.exchange,
      event.position_update.symbol,
      [&event](Instrument& instrument) { instrument.on(event); }))
    LOG(WARNING) << "Received position update for unknown {"
        "exchange=\"" << event.position_update.exchange << "\", "
        "symbol=\"" << event.position_update.symbol << "\""
        "}";
}

// Note! Order updates may be sent live or during the download phase.
// During the download phase we will receive everything previously
// sent to the gateway. Thus, by managing reconnection and download
// events, we're able to recover the state at which we left off if
// either the gateway restarts (or reconnects) or the client for
// whatever reason has to be restarted.
void BaseStrategy::on(const roq::OrderUpdateEvent& event) {
  if (_gateway.is_downloading()) {
    if (!apply(
        event.order_update.exchange,
        event.order_update.symbol,
        [this, &event](Instrument& instrument) {
            _gateway.on(event, &instrument); }))
      LOG(WARNING) << "Received order update for unknown {"
          "exchange=\"" << event.order_update.exchange << "\", "
          "symbol=\"" << event.order_update.symbol << "\""
          "}";
  } else {
    _gateway.on(event);
  }
}

void BaseStrategy::on(const roq::TradeUpdateEvent& event) {
  apply(
      event.trade_update.exchange,
      event.trade_update.symbol,
      [this, &event](Instrument& instrument) { instrument.on(event); });
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
      event.market_by_price.symbol,
      [this, &event](Instrument& instrument) {
        instrument.on(event);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::TradeSummaryEvent& event) {
  apply(
      event.trade_summary.exchange,
      event.trade_summary.symbol,
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
    const std::string& symbol,
    std::function<void(Instrument&)> function) {
  auto iter = _lookup.find(symbol);
  if (iter == _lookup.end())
    return false;
  function(*(*iter).second);
  return true;
}

}  // namespace common
}  // namespace examples
