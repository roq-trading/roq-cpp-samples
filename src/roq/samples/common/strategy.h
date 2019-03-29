/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "roq/client.h"

#include "roq/samples/common/account.h"
#include "roq/samples/common/config.h"
#include "roq/samples/common/instrument.h"

namespace roq {
namespace samples {
namespace common {

class Strategy : public client::Handler {
 public:
  Strategy(
      client::Dispatcher& dispatcher,
      const Config& config);

  inline size_t size() const {
    return _instruments.size();
  }
  inline Instrument& at(size_t index) {
    return _instruments.at(index);
  }
  inline const Instrument& at(size_t index) const {
    return _instruments.at(index);
  }
  inline bool is_ready() const {
    return _all_accounts_ready && _all_instruments_ready;
  }

 protected:
  virtual void update(std::chrono::nanoseconds now) {}
  virtual void update(const CreateOrderAck&) {}
  virtual void update(const OrderUpdate&) {}
  virtual void update(const TradeUpdate&) {}
  virtual void update(const MarketData&) = 0;

 protected:
  void on(const StartEvent&) final;
  void on(const StopEvent&) final;
  void on(const TimerEvent&) final;
  void on(const ConnectionStatusEvent&) final;
  void on(const BatchBeginEvent&) final;
  void on(const BatchEndEvent& event) final;
  void on(const MarketDataStatusEvent& event) final;
  void on(const OrderManagerStatusEvent& event) final;
  void on(const DownloadBeginEvent& event) final;
  void on(const DownloadEndEvent& event) final;
  void on(const ReferenceDataEvent& event) final;
  void on(const MarketStatusEvent& event) final;
  void on(const SessionStatisticsEvent& event) final;
  void on(const DailyStatisticsEvent& event) final;
  void on(const MarketByPriceEvent& event) final;
  void on(const TradeSummaryEvent& event) final;
  void on(const PositionUpdateEvent& event) final;
  void on(const OrderUpdateEvent& event) final;
  void on(const TradeUpdateEvent& event) final;
  void on(const CreateOrderAckEvent& event) final;
  void on(const ModifyOrderAckEvent& event) final;
  void on(const CancelOrderAckEvent& event) final;
  void on(const CustomMessageEvent& event) final;

  void write_metrics(std::ostream& stream) override;

 private:
  client::Dispatcher& _dispatcher;
  std::vector<Account> _accounts;
  std::unordered_map<std::string, Account *> _accounts_by_name;
  std::vector<Instrument> _instruments;
  std::unordered_map<std::string, Instrument *> _instruments_by_name;
  bool _all_accounts_ready = false;
  bool _all_instruments_ready = false;
  std::vector<bool> _market_data_updated;
};

}  // namespace common
}  // namespace samples
}  // namespace roq
