/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <array>
#include <limits>
#include <memory>

#include "roq/api.h"

#include "roq/client/depth_builder.h"

#include "roq/samples/example-3/utilities.h"

namespace roq {
namespace samples {
namespace example_3 {

class Instrument final {
 public:
  static constexpr size_t MAX_DEPTH = 3u;
  using Depth = std::array<Layer, MAX_DEPTH>;

  Instrument(
      const std::string_view &exchange,
      const std::string_view &symbol,
      const std::string_view &account);

  Instrument(Instrument &&) = default;
  Instrument(const Instrument &) = delete;

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
  void check_ready();

  void reset();

  void validate(const Depth &depth);

 private:
  const std::string_view exchange_;
  const std::string_view symbol_;
  const std::string_view account_;
  ConnectionStatus connection_status_ = ConnectionStatus::DISCONNECTED;
  bool download_ = false;
  double tick_size_ = NaN;
  double min_trade_vol_ = NaN;
  double multiplier_ = NaN;
  TradingStatus trading_status_ = TradingStatus::UNDEFINED;
  GatewayStatus market_data_status_ = GatewayStatus::DISCONNECTED;
  GatewayStatus order_manager_status_ = GatewayStatus::DISCONNECTED;
  Depth depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  double long_position_ = {};
  double short_position_ = {};
  bool ready_ = false;
  uint32_t last_order_id_ = {};
  double last_traded_quantity_ = {};
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
