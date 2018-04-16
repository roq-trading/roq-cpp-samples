/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common/config.h"
#include "common/gateway.h"
#include "common/instrument.h"

namespace examples {
namespace common {

class BaseStrategy : public roq::Strategy {
 public:
  explicit BaseStrategy(
      roq::Strategy::Dispatcher& dispatcher,
      const std::string& gateway,
      const Config& config);
  const std::string& get_order_template(bool close) const {
    return _gateway.get_order_template(close);
  }
  size_t size() const { return _instruments.size(); }
  Instrument& at(size_t index) {
    return _instruments[index];
  }
  const Instrument& at(size_t index) const {
    return _instruments[index];
  }
  bool is_ready() const;

 protected:
  virtual void update(const MarketData&) = 0;

 private:
  const roq::Strategy::subscriptions_t& get_subscriptions() const override {
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
  // - download
  void on(const roq::DownloadBeginEvent&) override;
  void on(const roq::DownloadEndEvent&) override;
  // - order manager or market data
  void on(const roq::GatewayStatusEvent&) override;
  // - order manager update
  void on(const roq::ReferenceDataEvent&) override;
  void on(const roq::MarketStatusEvent&) override;
  void on(const roq::PositionUpdateEvent&) override;
  void on(const roq::OrderUpdateEvent&) override;
  void on(const roq::TradeUpdateEvent&) override {}
  // - order manager response
  void on(const roq::CreateOrderAckEvent&) override;
  void on(const roq::ModifyOrderAckEvent&) override;
  void on(const roq::CancelOrderAckEvent&) override;
  // - market data update
  void on(const roq::MarketByPriceEvent&) override;
  void on(const roq::TradeSummaryEvent&) override;
  // utilities
  bool apply(
      const std::string& exchange,
      const std::string& symbol,
      std::function<void(Instrument&)> function);

 private:
  Gateway _gateway;
  std::vector<Instrument> _instruments;
  const std::unordered_map<std::string, Instrument *> _lookup;
  const roq::Strategy::subscriptions_t _subscriptions;
  bool _instruments_ready = false;
  std::unordered_set<Instrument *> _market_data_updated;
};

}  // namespace common
}  // namespace examples
