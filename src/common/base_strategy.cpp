/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/base_strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

namespace examples {
namespace common {

namespace {
static std::vector<Account> create_accounts(
    roq::Strategy::Dispatcher& dispatcher,
    const Config& config) {
  std::vector<Account> result;
  // HANS -- this is wrong -- need new config format
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    for (const auto& iter : instrument.accounts)
      result.emplace_back(dispatcher, iter.first, config);
  }
  return result;
}
static std::unordered_map<std::string, Account *>
create_accounts_by_name(
    std::vector<Account>& accounts) {
  std::unordered_map<std::string, Account *> result;
  for (auto& iter : accounts) {
    result.emplace(iter.get_name(), &iter);
  }
  return result;
}
static std::vector<Instrument> create_instruments(
    const Config& config,
    std::vector<Account>& accounts) {
  std::vector<Instrument> result;
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    std::vector<Position *> positions;
    for (auto& account : accounts)
      positions.emplace_back(
          &account.get_position(
              instrument.exchange,
              instrument.symbol));
    result.emplace_back(
      i,
      instrument.exchange,
      instrument.symbol,
      instrument.risk_limit,
      instrument.tick_size,
      instrument.multiplier,
      std::move(positions));
  }
  return result;
}
static std::unordered_map<std::string, Instrument *>
create_instruments_by_name(
    std::vector<Instrument>& instruments) {
  std::unordered_map<std::string, Instrument *> result;
  for (auto& instrument : instruments)
    result.emplace(instrument.get_symbol(), &instrument);
  return result;
}
static roq::Strategy::subscriptions_t create_subscriptions(
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
      _accounts(create_accounts(dispatcher, config)),
      _accounts_by_name(create_accounts_by_name(_accounts)),
      _instruments(create_instruments(config, _accounts)),
      _instruments_by_name(create_instruments_by_name(_instruments)),
      _subscriptions(create_subscriptions(gateway, _instruments)) {
  // DEBUG
  for (const auto& iter : _accounts_by_name) {
    LOG(INFO) << iter.first << "=" << *iter.second;
  }
  for (const auto& iter : _instruments_by_name) {
    LOG(INFO) << iter.first << "=" << *iter.second;
  }
}

// event handlers

// timer

void BaseStrategy::on(const roq::TimerEvent& event) {
  for (auto& account : _accounts)
    account.on(event);
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

// market data

void BaseStrategy::on(const roq::MarketDataStatusEvent& event) {
  _gateway.on(event);
}

void BaseStrategy::on(const roq::MarketByPriceEvent& event) {
  apply(
      event.market_by_price.exchange,
      event.market_by_price.symbol,
      [&](Instrument& instrument) {
        instrument.on(event);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::TradeSummaryEvent& event) {
  apply(
      event.trade_summary.exchange,
      event.trade_summary.symbol,
      [&](Instrument& instrument) {
        instrument.on(event);
        _market_data_updated.insert(&instrument);
      });
}

// download

void BaseStrategy::on(const roq::DownloadBeginEvent& event) {
  const auto& download_begin = event.download_begin;
  apply(
      download_begin.account,
      [&](Account& account) {
          account.on(download_begin); });
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
  const auto& download_end = event.download_end;
  apply(
      download_end.account,
      [&](Account& account) {
          account.on(download_end); });
}

// order manager

void BaseStrategy::on(const roq::ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  apply(
      reference_data.exchange,
      reference_data.symbol,
      [&](Instrument& instrument) {
          instrument.on(event); });  // HANS -- why event?
}

void BaseStrategy::on(const roq::MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  if (apply(
      market_status.exchange,
      market_status.symbol,
      [&](Instrument& instrument) {
          instrument.on(event); })) {  // HANS -- why event?
    // are *all* instruments ready for trading?
    bool ready = true;
    for (const auto& instrument : _instruments)
      if (instrument.can_trade())
        ready = instrument.is_ready() ? ready : false;
    _instruments_ready = ready;
  }
}

void BaseStrategy::on(const roq::PositionUpdateEvent& event) {
  const auto& position_update = event.position_update;
  apply(
      position_update.account,
      [&](Account& account) {
          account.on(position_update); });
}

void BaseStrategy::on(const roq::OrderUpdateEvent& event) {
  const auto& order_update = event.order_update;
  apply(
      order_update.account,
      [&](Account& account) {
          account.on(order_update); });
}

void BaseStrategy::on(const roq::TradeUpdateEvent& event) {
  const auto& trade_update = event.trade_update;
  apply(
      trade_update.account,
      [&](Account& account) {
          account.on(trade_update); });
}

void BaseStrategy::on(const roq::OrderManagerStatusEvent& event) {
  const auto& order_manager_status = event.order_manager_status;
  if (apply(
      order_manager_status.account,
      [&](Account& account) {
          account.on(order_manager_status); })) {
    // are *all* accounts ready for trading?
    bool ready = true;
    for (const auto& account : _accounts)
      ready = account.is_ready() ? ready : false;
    _accounts_ready = ready;
    LOG(INFO) << "accounts_ready=" << (_accounts_ready ? "true" : "false");
  }
}

// request-response

void BaseStrategy::on(const roq::CreateOrderAckEvent& event) {
  const auto& create_order_ack = event.create_order_ack;
  apply(
      create_order_ack.account,
      [&](Account& account) {
          account.on(create_order_ack); });
}

void BaseStrategy::on(const roq::ModifyOrderAckEvent& event) {
  const auto& modify_order_ack = event.modify_order_ack;
  apply(
      modify_order_ack.account,
      [&](Account& account) {
          account.on(modify_order_ack); });
}

void BaseStrategy::on(const roq::CancelOrderAckEvent& event) {
  const auto& cancel_order_ack = event.cancel_order_ack;
  apply(
      cancel_order_ack.account,
      [&](Account& account) {
          account.on(cancel_order_ack); });
}

bool BaseStrategy::is_ready() const {
  return _accounts_ready && _instruments_ready;
}

// general utilities

bool BaseStrategy::apply(
    const std::string& exchange,
    const std::string& symbol,
    std::function<void(Instrument&)> function) {
  auto iter = _instruments_by_name.find(symbol);
  if (iter == _instruments_by_name.end())
    return false;
  function(*(*iter).second);
  return true;
}

bool BaseStrategy::apply(
    const std::string& account,
    std::function<void(Account&)> function) {
  auto iter = _accounts_by_name.find(account);
  if (iter == _accounts_by_name.end())
    return false;
  function(*(*iter).second);
  return true;
}

}  // namespace common
}  // namespace examples
