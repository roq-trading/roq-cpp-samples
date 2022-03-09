/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <array>
#include <limits>
#include <memory>

#include "roq/api.hpp"

#include "roq/cache/market_by_price.hpp"

namespace roq {
namespace samples {
namespace example_3 {

class Instrument final {
 public:
  static const constexpr size_t MAX_DEPTH = 3;

  using Depth = std::array<Layer, MAX_DEPTH>;

  Instrument(const std::string_view &exchange, const std::string_view &symbol, const std::string_view &account);

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

  void operator()(const Connected &);
  void operator()(const Disconnected &);
  void operator()(const DownloadBegin &);
  void operator()(const DownloadEnd &);
  void operator()(const GatewayStatus &);
  void operator()(const ReferenceData &);
  void operator()(const MarketStatus &);
  void operator()(const MarketByPriceUpdate &);
  void operator()(const OrderUpdate &);
  void operator()(const PositionUpdate &);

 protected:
  void check_ready();

  void reset();

  void validate(const Depth &);

 private:
  const std::string_view exchange_;
  const std::string_view symbol_;
  const std::string_view account_;
  bool connected_ = false;
  bool download_ = false;
  double tick_size_ = NaN;
  double min_trade_vol_ = NaN;
  double multiplier_ = NaN;
  TradingStatus trading_status_ = {};
  bool market_data_ = {};
  bool order_management_ = {};
  Depth depth_;
  std::unique_ptr<cache::MarketByPrice> market_by_price_;
  double long_position_ = {};
  double short_position_ = {};
  bool ready_ = false;
  uint32_t last_order_id_ = {};
  double last_traded_quantity_ = {};
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
