/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/common/strategy.h"

#include <utility>

#include "roq/logging.h"
#include "roq/stream.h"

namespace roq {
namespace samples {
namespace common {

namespace {
static auto create_accounts(
    client::Dispatcher& dispatcher,
    const Config& config) {
  std::vector<Account> result;
  for (const auto& instrument : config.instruments) {
    for (const auto& iter : instrument.accounts) {
      // TODO(thraneh): shouldn't we check for duplicates???
      result.emplace_back(
          dispatcher,
          iter.first,
          config);
    }
  }
  return result;
}
static auto create_instruments(
    client::Dispatcher& /*dispatcher*/,
    const Config& config,
    const std::unordered_map<std::string, Account *>& accounts_by_name) {
  std::vector<Instrument> result;
  for (size_t i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    std::vector<Position *> positions;
    for (auto& iter : instrument.accounts) {
      auto& account = accounts_by_name.at(iter.first);
      auto& position = account->get_position(
          instrument.exchange,
          instrument.symbol);
      positions.emplace_back(&position);
    }
    result.emplace_back(
        i,
        instrument.exchange,
        instrument.symbol,
        instrument.net_limit,
        instrument.tick_size,
        instrument.multiplier,
        std::move(positions));
  }
  return result;
}
}  // namespace

namespace {
template <typename T, typename F>
static auto create_by_name(
    std::vector<T>& values,
    F get_name) {
  std::unordered_map<std::string, T *> result;
  for (auto& iter : values)
    result.emplace(get_name(iter), &iter);
  return result;
}
}  // namespace

namespace {
template <typename F>
inline bool apply(
    std::unordered_map<std::string, Instrument *>& instruments_by_name,
    const std::string& exchange,
    const std::string& symbol,
    F function) {
  auto iter = instruments_by_name.find(symbol);
  if (iter == instruments_by_name.end()) {
    VLOG(3) << "Got update for unknown "
      "exchange=\"" << exchange << "\", symbol=\"" << symbol << "\"";
    return false;
  }
  function(*(*iter).second);
  return true;
}
template <typename F>
inline bool apply(
    std::unordered_map<std::string, Account *>& accounts_by_name,
    const std::string& account,
    F function) {
  auto iter = accounts_by_name.find(account);
  if (iter == accounts_by_name.end()) {
    LOG(WARNING) << "Got update for unknown "
      "account=\"" << account << "\"";
    return false;
  }
  function(*(*iter).second);
  return true;
}
}  // namespace

Strategy::Strategy(
    client::Dispatcher& dispatcher,
    const Config& config)
    : _dispatcher(dispatcher),
      _accounts(
          create_accounts(
              dispatcher,
              config)),
      _accounts_by_name(
          create_by_name(
              _accounts,
              [](Account& account) {
                return account.get_name(); })),
      _instruments(
          create_instruments(
              dispatcher,
              config,
              _accounts_by_name)),
      _instruments_by_name(
          create_by_name(
              _instruments,
              [](Instrument& instrument) {
                return instrument.get_symbol(); })) {
  _market_data_updated.resize(_instruments.size());
}

void Strategy::on(const TimerEvent& event) {
  for (auto& account : _accounts)
    account.on(event);
  // auto now = ...?
  // update(now);
}

void Strategy::on(const ConnectionStatusEvent&) {
}

void Strategy::on(const BatchBeginEvent&) {
  bool any = false;
  for (auto iter : _market_data_updated)
    any |= iter;
  LOG_IF(FATAL, any) << "Unexpected";
}

void Strategy::on(const BatchEndEvent&) {
  auto size = _market_data_updated.size();
  for (size_t i = 0; i < size; ++i) {
    if (_market_data_updated[i])
      update(_instruments[i].get_market_data());
  }
  _market_data_updated.assign(size, false);
}

void Strategy::on(const MarketDataStatusEvent& event) {
  const auto& market_data_status = event.market_data_status;
  for (auto& instrument : _instruments)
    instrument.on(market_data_status);
}

void Strategy::on(const OrderManagerStatusEvent& event) {
  const auto& order_manager_status = event.order_manager_status;
  if (apply(
        _accounts_by_name,
        order_manager_status.account,
        [&](Account& account) {
      account.on(order_manager_status); })) {
    // are *all* accounts ready for trading?
    bool ready = true;
    for (const auto& account : _accounts)
    ready = account.is_ready() ? ready : false;
    if (_all_accounts_ready != ready) {
      _all_accounts_ready = ready;
      LOG(INFO) << "all_accounts_ready=" <<
        (_all_accounts_ready ? "true" : "false");
    }
  }
}

void Strategy::on(const DownloadBeginEvent& event) {
  VLOG(1) << "DownloadBeginEvent " << event;
  const auto& download_begin = event.download_begin;
  if (download_begin.account && std::strlen(download_begin.account)) {
    apply(
        _accounts_by_name,
        download_begin.account,
        [&](Account& account) {
      account.on(download_begin); });
  } else {
    LOG(INFO) << "Reference data download starting...";
  }
}

void Strategy::on(const DownloadEndEvent& event) {
  VLOG(1) << "DownloadEndEvent " << event;
  const auto& download_end = event.download_end;
  if (download_end.account && std::strlen(download_end.account)) {
    LOG(INFO) << "Account download completed:";
    apply(
        _accounts_by_name,
        download_end.account,
        [&](Account& account) {
      account.on(download_end);
      /*
      LOG(INFO) << "Account download completed: "
        "account=" << account; });
      */
      });
  } else {
    /*
    LOG(INFO) << "Reference data download completed: "
      "instruments=" << _instruments;
    */
  }
}

void Strategy::on(const ReferenceDataEvent& event) {
  const auto& reference_data = event.reference_data;
  apply(
      _instruments_by_name,
      reference_data.exchange,
      reference_data.symbol,
      [&](Instrument& instrument) {
    instrument.on(reference_data); });
}

void Strategy::on(const MarketStatusEvent& event) {
  const auto& market_status = event.market_status;
  if (apply(
        _instruments_by_name,
        market_status.exchange,
        market_status.symbol,
        [&](Instrument& instrument) {
      instrument.on(market_status); })) {
    // are *all* instruments ready for trading?
    bool ready = true;
    for (const auto& instrument : _instruments)
      if (instrument.can_trade())
        ready = instrument.is_ready() ? ready : false;
    if (_all_instruments_ready != ready) {
      _all_instruments_ready = ready;
      LOG(INFO) << "all_instruments_ready=" <<
        (_all_instruments_ready ? "true" : "false");
    }
  }
}

void Strategy::on(const SessionStatisticsEvent& event) {
  const auto& session_statistics = event.session_statistics;
  apply(
      _instruments_by_name,
      session_statistics.exchange,
      session_statistics.symbol,
      [&](Instrument& instrument) {
    instrument.on(session_statistics);
    _market_data_updated[instrument.index()] = true;
  });
}

void Strategy::on(const DailyStatisticsEvent& event) {
  const auto& daily_statistics = event.daily_statistics;
  apply(
      _instruments_by_name,
      daily_statistics.exchange,
      daily_statistics.symbol,
      [&](Instrument& instrument) {
    instrument.on(daily_statistics);
    _market_data_updated[instrument.index()] = true;
  });
}

void Strategy::on(const MarketByPriceEvent& event) {
  const auto& market_by_price = event.market_by_price;
  apply(
      _instruments_by_name,
      market_by_price.exchange,
      market_by_price.symbol,
      [&](Instrument& instrument) {
    instrument.on(market_by_price);
    _market_data_updated[instrument.index()] = true;
  });
}

void Strategy::on(const TradeSummaryEvent& event) {
  const auto& trade_summary = event.trade_summary;
  apply(
      _instruments_by_name,
      trade_summary.exchange,
      trade_summary.symbol,
      [&](Instrument& instrument) {
    instrument.on(trade_summary);
    _market_data_updated[instrument.index()] = true;
  });
}

void Strategy::on(const PositionUpdateEvent& event) {
  const auto& position_update = event.position_update;
  apply(
      _accounts_by_name,
      position_update.account,
      [&](Account& account) {
    account.on(position_update); });
}

void Strategy::on(const OrderUpdateEvent& event) {
  const auto& order_update = event.order_update;
  apply(
      _accounts_by_name,
      order_update.account,
      [&](Account& account) {
    account.on(order_update);
    if (account.is_downloading()) {
      // VLOG(2) << "account=" << account;
    } else {
      // LOG(INFO) << "account=" << account;
    }
  });
  update(order_update);
}

void Strategy::on(const TradeUpdateEvent& event) {
  const auto& trade_update = event.trade_update;
  apply(
      _accounts_by_name,
      trade_update.account,
      [&](Account& account) {
    account.on(trade_update);
  });
  update(trade_update);
}

void Strategy::on(const CreateOrderAckEvent& event) {
  const auto& create_order_ack = event.create_order_ack;
  apply(
      _accounts_by_name,
      create_order_ack.account,
      [&](Account& account) {
    account.on(create_order_ack); });
  update(create_order_ack);
}

void Strategy::on(const ModifyOrderAckEvent& event) {
  const auto& modify_order_ack = event.modify_order_ack;
  apply(
      _accounts_by_name,
      modify_order_ack.account,
      [&](Account& account) {
    account.on(modify_order_ack); });
}

void Strategy::on(const CancelOrderAckEvent& event) {
  const auto& cancel_order_ack = event.cancel_order_ack;
  apply(
      _accounts_by_name,
      cancel_order_ack.account,
      [&](Account& account) {
    account.on(cancel_order_ack); });
}

}  // namespace common
}  // namespace samples
}  // namespace roq
