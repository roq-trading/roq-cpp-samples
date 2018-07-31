/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/account.h"
#include "common/config.h"
#include "common/instrument.h"

namespace examples {
namespace common {

class BaseStrategy : public roq::Client {
 public:
  explicit BaseStrategy(
      roq::Client::Dispatcher& dispatcher,
      const std::string& gateway,
      const Config& config);

  size_t size() const { return _instruments.size(); }
  Instrument& at(size_t index) {
    return *_instruments[index];
  }
  const Instrument& at(size_t index) const {
    return *_instruments[index];
  }
  bool is_ready() const;

 protected:
  virtual void update(std::chrono::system_clock::time_point now) = 0;
  virtual void update(const MarketData&) = 0;

 private:
  const roq::Subscriptions& get_subscriptions() const final override {
    return _subscriptions;
  }
  // api event handlers:
  // - timer
  void on(const roq::TimerEvent&) final override;
  // - connection
  void on(const roq::ConnectionStatusEvent&) final override {}
  // - batch
  void on(const roq::BatchBeginEvent&) final override;
  void on(const roq::BatchEndEvent&) final override;
  // - market data
  void on(const roq::MarketDataStatusEvent&) final override;
  // - session update
  void on(const roq::SessionStatisticsEvent&) final override;
  void on(const roq::DailyStatisticsEvent&) final override;
  // - market data update
  void on(const roq::MarketByPriceEvent&) final override;
  void on(const roq::TradeSummaryEvent&) final override;
  // - order manager
  void on(const roq::DownloadBeginEvent&) final override;
  void on(const roq::DownloadEndEvent&) final override;
  void on(const roq::ReferenceDataEvent&) final override;
  void on(const roq::MarketStatusEvent&) final override;
  void on(const roq::PositionUpdateEvent&) final override;
  void on(const roq::OrderUpdateEvent&) final override;
  void on(const roq::TradeUpdateEvent&) override;  // FIXME(thraneh): final
  void on(const roq::OrderManagerStatusEvent&) final override;
  // - order manager response
  void on(const roq::CreateOrderAckEvent&) final override;
  void on(const roq::ModifyOrderAckEvent&) final override;
  void on(const roq::CancelOrderAckEvent&) final override;
  // utilities
  bool apply(
      const std::string& exchange,
      const std::string& symbol,
      std::function<void(Instrument&)> function);
  bool apply(
      const std::string& account,
      std::function<void(Account&)> function);

 private:
  std::vector<std::shared_ptr<Account> > _accounts;
  std::unordered_map<std::string, std::shared_ptr<Account> > _accounts_by_name;
  std::vector<std::shared_ptr<Instrument> > _instruments;
  const std::unordered_map<std::string, std::shared_ptr<Instrument> > _instruments_by_name;

  const roq::Subscriptions _subscriptions;

  bool _all_instruments_ready = false;
  bool _all_accounts_ready = false;

  std::unordered_set<Instrument *> _market_data_updated;
};

}  // namespace common
}  // namespace examples
