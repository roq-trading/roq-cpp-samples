/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <limits>
#include <memory>

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_2 {

// helper class caching instrument specific information

class Instrument final {
 public:
  Instrument(const std::string_view &exchange, const std::string_view &symbol);

  bool is_ready() const { return ready_; }

  void operator()(const Connection &connection);
  void operator()(const DownloadBegin &download_begin);
  void operator()(const DownloadEnd &download_end);
  void operator()(const MarketDataStatus &market_data_status);
  void operator()(const ReferenceData &reference_data);
  void operator()(const MarketStatus &market_status);
  void operator()(const MarketByPriceUpdate &market_by_price_update);
  void operator()(const MarketByOrderUpdate &market_by_order_update);

 protected:
  void update_model();

  void checkready_();

  void reset();

 private:
  static const size_t MAX_DEPTH = 2;
  const std::string_view exchange_;
  const std::string_view symbol_;
  ConnectionStatus connection_status_ = ConnectionStatus::DISCONNECTED;
  bool download_ = false;
  double tick_size_ = std::numeric_limits<double>::quiet_NaN();
  double min_trade_vol_ = std::numeric_limits<double>::quiet_NaN();
  double multiplier_ = std::numeric_limits<double>::quiet_NaN();
  TradingStatus trading_status_ = TradingStatus::UNDEFINED;
  GatewayStatus market_data_status_ = GatewayStatus::DISCONNECTED;
  std::array<Layer, MAX_DEPTH> depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  double mid_price_ = std::numeric_limits<double>::quiet_NaN();
  double avg_price_ = std::numeric_limits<double>::quiet_NaN();
  bool ready_ = false;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
