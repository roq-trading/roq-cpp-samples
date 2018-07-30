/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/base_strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <gflags/gflags.h>

#include <algorithm>

DEFINE_bool(beep_on_trade, false, "New trade generates a beep.");

namespace examples {
namespace common {

namespace {
static std::vector<std::shared_ptr<Account> > create_accounts(
    roq::Client::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config) {
  std::vector<std::shared_ptr<Account> > result;
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    for (const auto& iter : instrument.accounts)
      result.emplace_back(
          std::make_shared<Account>(
              dispatcher,
              gateway,
              iter.first,
              config));
  }
  return result;
}
static std::unordered_map<std::string, std::shared_ptr<Account> >
create_accounts_by_name(
    std::vector<std::shared_ptr<Account> >& accounts) {
  std::unordered_map<std::string, std::shared_ptr<Account> > result;
  for (auto& iter : accounts) {
    result.emplace(iter->get_name(), iter);
  }
  return result;
}
static std::vector<std::shared_ptr<Instrument> > create_instruments(
    const Config& config,
    std::unordered_map<std::string, std::shared_ptr<Account> >& accounts_by_name) {
  std::vector<std::shared_ptr<Instrument> > result;
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    std::vector<std::shared_ptr<Position> > positions;
    for (auto& iter : instrument.accounts) {
      auto& account = accounts_by_name.at(iter.first);
      positions.emplace_back(
          account->get_position(
              instrument.exchange,
              instrument.symbol));
    }
    result.emplace_back(
        std::make_shared<Instrument>(
            i,
            instrument.exchange,
            instrument.symbol,
            instrument.tick_size,
            instrument.multiplier,
            instrument.net_limit,
            std::move(positions)));
  }
  return result;
}
static std::unordered_map<std::string, std::shared_ptr<Instrument> >
create_instruments_by_name(
    std::vector<std::shared_ptr<Instrument> >& instruments) {
  std::unordered_map<std::string, std::shared_ptr<Instrument> > result;
  for (auto& instrument : instruments)
    result.emplace(instrument->get_symbol(), instrument);
  return result;
}
static roq::Subscriptions create_subscriptions(
    const Config& config,
    const std::string& gateway,
    const std::vector<std::shared_ptr<Instrument> >& instruments) {
  roq::Subscriptions result;
  auto& subscription = result[gateway];
  // symbols by exchange
  for (const auto& instrument : instruments) {
    const auto& exchange = instrument->get_exchange();
    const auto& symbol = instrument->get_symbol();
    subscription.symbols_by_exchange[exchange].emplace(symbol);
  }
  // accounts
  for (const auto& instrument : config.instruments)
    for (const auto& account : instrument.accounts)
      subscription.accounts.emplace(account.first);
  return result;
}
}  // namespace

namespace {
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<std::shared_ptr<Account> >& accounts) {
  stream << "[";
  bool first = true;
  for (const auto& account : accounts) {
    if (!first)
      stream << ", ";
    stream << *account;
    first = false;
  }
  return stream << "]";
}
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<std::shared_ptr<Instrument> >& instruments) {
  stream << "[";
  bool first = true;
  for (const auto& instrument : instruments) {
    if (!first)
      stream << ", ";
    stream << *instrument;
    first = false;
  }
  return stream << "]";
}
}  // namespace
BaseStrategy::BaseStrategy(
    roq::Client::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config)
    : _accounts(create_accounts(dispatcher, gateway, config)),
      _accounts_by_name(create_accounts_by_name(_accounts)),
      _instruments(create_instruments(config, _accounts_by_name)),
      _instruments_by_name(create_instruments_by_name(_instruments)),
      _subscriptions(create_subscriptions(config, gateway, _instruments)) {
  LOG(INFO) << "accounts=" << _accounts;
  LOG(INFO) << "instruments=" << _instruments;
}

// event handlers

// timer

void BaseStrategy::on(const roq::TimerEvent& event) {
  for (auto& account : _accounts)
    account->on(event);
  auto now = std::chrono::system_clock::now();
  update(now);
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
  const auto& market_data_status = event.market_data_status;
  for (auto& instrument : _instruments)
    instrument->on(market_data_status);
}

void BaseStrategy::on(const roq::SessionStatisticsEvent& event) {
  const auto& session_statistics = event.session_statistics;
  apply(
      session_statistics.exchange,
      session_statistics.symbol,
      [&](Instrument& instrument) {
        instrument.on(session_statistics);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::DailyStatisticsEvent& event) {
  const auto& daily_statistics = event.daily_statistics;
  apply(
      daily_statistics.exchange,
      daily_statistics.symbol,
      [&](Instrument& instrument) {
        instrument.on(daily_statistics);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  apply(
      market_by_price.exchange,
      market_by_price.symbol,
      [&](Instrument& instrument) {
        instrument.on(market_by_price);
        _market_data_updated.insert(&instrument);
      });
}

void BaseStrategy::on(const roq::TradeSummaryEvent& event) {
  const auto& trade_summary = event.trade_summary;
  apply(
      trade_summary.exchange,
      trade_summary.symbol,
      [&](Instrument& instrument) {
        instrument.on(trade_summary);
        _market_data_updated.insert(&instrument);
      });
}

// download

void BaseStrategy::on(const roq::DownloadBeginEvent& event) {
  const auto& download_begin = event.download_begin;
  if (download_begin.account && std::strlen(download_begin.account)) {
    apply(
        download_begin.account,
        [&](Account& account) {
            account.on(download_begin); });
  } else {
    LOG(INFO) << "Reference data download starting...";
  }
}

void BaseStrategy::on(const roq::DownloadEndEvent& event) {
  const auto& download_end = event.download_end;
  if (download_end.account && std::strlen(download_end.account)) {
    LOG(INFO) << "Account download completed:";
    apply(
        download_end.account,
        [&](Account& account) {
            account.on(download_end);
            LOG(INFO) << "Account download completed: "
              "account=" << account; });
  } else {
    LOG(INFO) << "Reference data download completed: "
      "instruments=" << _instruments;
  }
}

// order manager

void BaseStrategy::on(const roq::ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  apply(
      reference_data.exchange,
      reference_data.symbol,
      [&](Instrument& instrument) {
          instrument.on(reference_data); });
}

void BaseStrategy::on(const roq::MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  if (apply(
      market_status.exchange,
      market_status.symbol,
      [&](Instrument& instrument) {
          instrument.on(market_status); })) {
    // are *all* instruments ready for trading?
    bool ready = true;
    for (const auto& instrument : _instruments)
      if (instrument->can_trade())
        ready = instrument->is_ready() ? ready : false;
    if (_all_instruments_ready != ready) {
      _all_instruments_ready = ready;
      LOG(INFO) << "all_instruments_ready=" <<
        (_all_instruments_ready ? "true" : "false");
    }
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
        account.on(order_update);
        if (account.is_downloading()) {
          VLOG(2) << "account=" << account;
        } else {
          LOG(INFO) << "account=" << account;
        }
      });
}

void BaseStrategy::on(const roq::TradeUpdateEvent& event) {
  const auto& trade_update = event.trade_update;
  apply(
      trade_update.account,
      [&](Account& account) {
        account.on(trade_update);
        if (FLAGS_beep_on_trade && !account.is_downloading()) {
          putc('\a', stdout);
        }
      });
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
      ready = account->is_ready() ? ready : false;
    if (_all_accounts_ready != ready) {
      _all_accounts_ready = ready;
      LOG(INFO) << "all_accounts_ready=" <<
        (_all_accounts_ready ? "true" : "false");
    }
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
  return _all_accounts_ready && _all_instruments_ready;
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
  if (iter == _accounts_by_name.end()) {
    LOG(WARNING) << "Got update for unknown account=\"" << account << "\"";
    return false;
  }
  function(*(*iter).second);
  return true;
}

}  // namespace common
}  // namespace examples
