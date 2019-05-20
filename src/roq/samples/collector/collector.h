/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <vector>

#include "roq/client.h"

namespace roq {
namespace samples {
namespace collector {

class Collector final : public roq::client::Handler {
 public:
  explicit Collector(roq::client::Dispatcher& dispatcher);

 protected:
  void on(const roq::ConnectionStatusEvent&) override;
  void on(const roq::BatchBeginEvent&) override;
  void on(const roq::BatchEndEvent& event) override;
  void on(const roq::MarketDataStatusEvent& event) override;
  void on(const roq::OrderManagerStatusEvent& event) override;
  void on(const roq::DownloadBeginEvent& event) override;
  void on(const roq::DownloadEndEvent& event) override;
  void on(const roq::ReferenceDataEvent& event) override;
  void on(const roq::MarketByPriceEvent& event) override;

 private:
  roq::client::Dispatcher& _dispatcher;
  bool _download = false;
  uint32_t _order_id = 0;
};

}  // namespace collector
}  // namespace samples
}  // namespace roq
