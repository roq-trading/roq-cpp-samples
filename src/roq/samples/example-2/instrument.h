/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <limits>
#include <memory>

#include "roq/api.h"
#include "roq/client.h"

#include "roq/samples/example-2/utilities.h"

namespace roq {
namespace samples {
namespace example_2 {

// helper class caching instrument specific information

class Instrument final {
 public:
  Instrument(const std::string_view &exchange, const std::string_view &symbol);

  Instrument(Instrument &&) = default;
  Instrument(const Instrument &) = delete;

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

  void check_ready();

  void reset();

 private:
  static constexpr size_t MAX_DEPTH = 2u;

  const std::string_view exchange_;
  const std::string_view symbol_;
  ConnectionStatus connection_status_ = ConnectionStatus::DISCONNECTED;
  bool download_ = false;
  double tick_size_ = NaN;
  double min_trade_vol_ = NaN;
  double multiplier_ = NaN;
  TradingStatus trading_status_ = TradingStatus::UNDEFINED;
  GatewayStatus market_data_status_ = GatewayStatus::DISCONNECTED;
  std::array<Layer, MAX_DEPTH> depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  double mid_price_ = NaN;
  double avg_price_ = NaN;
  bool ready_ = false;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
