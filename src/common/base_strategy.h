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
  const roq::Subscriptions& get_subscriptions() const override {
    return _subscriptions;
  }
  // api event handlers:
  // - timer
  void on(const roq::TimerEvent&) override;
  // - connection
  void on(const roq::ConnectionStatusEvent&) override {}
  // - batch
  void on(const roq::BatchBeginEvent&) override;
  void on(const roq::BatchEndEvent&) override;
  // - market data
  void on(const roq::MarketDataStatusEvent&) override;
  // - session update
  void on(const roq::SessionStatisticsEvent&) override;
  void on(const roq::DailyStatisticsEvent&) override;
  // - market data update
  void on(const roq::MarketByPriceEvent&) override;
  void on(const roq::TradeSummaryEvent&) override;
  // - order manager
  void on(const roq::DownloadBeginEvent&) override;
  void on(const roq::DownloadEndEvent&) override;
  void on(const roq::ReferenceDataEvent&) override;
  void on(const roq::MarketStatusEvent&) override;
  void on(const roq::PositionUpdateEvent&) override;
  void on(const roq::OrderUpdateEvent&) override;
  void on(const roq::TradeUpdateEvent&) override;
  void on(const roq::OrderManagerStatusEvent&) override;
  // - order manager response
  void on(const roq::CreateOrderAckEvent&) override;
  void on(const roq::ModifyOrderAckEvent&) override;
  void on(const roq::CancelOrderAckEvent&) override;
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
