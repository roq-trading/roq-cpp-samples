/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <array>
#include <limits>
#include <memory>

#include "roq/api.h"

#include "roq/client/depth_builder.h"

namespace roq {
namespace samples {
namespace example_3 {

class Instrument final {
 public:
  static constexpr size_t MAX_DEPTH = 3;
  using Depth = std::array<Layer, MAX_DEPTH>;

  Instrument(
      const std::string_view &exchange,
      const std::string_view &symbol,
      const std::string_view &account);

  operator const Depth &() const { return depth_; }

  bool is_ready() const { return ready_; }

  auto tick_size() const { return tick_size_; }

  auto min_trade_vol() const { return min_trade_vol_; }

  auto multiplier() const { return multiplier_; }

  auto is_market_open() const { return trading_status_ == TradingStatus::OPEN; }

  auto best_bid() const { return depth_[0].bid_price; }

  auto best_ask() const { return depth_[0].ask_price; }

  double position() const;

  bool can_trade(Side side) const;

  void operator()(const Connection &connection);

  void operator()(const DownloadBegin &download_begin);

  void operator()(const DownloadEnd &download_end);

  void operator()(const MarketDataStatus &market_data_status);

  void operator()(const OrderManagerStatus &order_manager_status);

  void operator()(const ReferenceData &reference_data);

  void operator()(const MarketStatus &market_status);

  void operator()(const MarketByPriceUpdate &market_by_price_update);

  void operator()(const MarketByOrderUpdate &market_by_order_update);

  void operator()(const OrderUpdate &order_update);

  void operator()(const PositionUpdate &position_update);

 protected:
  void checkready_();

  void reset();

  void validate(const Depth &depth);

 private:
  const std::string_view exchange_;
  const std::string_view symbol_;
  const std::string_view account_;
  ConnectionStatus connection_status_ = ConnectionStatus::DISCONNECTED;
  bool download_ = false;
  double tick_size_ = std::numeric_limits<double>::quiet_NaN();
  double min_trade_vol_ = std::numeric_limits<double>::quiet_NaN();
  double multiplier_ = std::numeric_limits<double>::quiet_NaN();
  TradingStatus trading_status_ = TradingStatus::UNDEFINED;
  GatewayStatus market_data_status_ = GatewayStatus::DISCONNECTED;
  GatewayStatus order_manager_status_ = GatewayStatus::DISCONNECTED;
  Depth depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  double long_position_ = 0.0;
  double short_position_ = 0.0;
  bool ready_ = false;
  uint32_t last_order_id_ = 0;
  double last_traded_quantity_ = 0.0;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
