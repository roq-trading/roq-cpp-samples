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
  static constexpr size_t const MAX_DEPTH = 3;

  using Depth = std::array<Layer, MAX_DEPTH>;

  Instrument(std::string_view const &exchange, std::string_view const &symbol, std::string_view const &account);

  Instrument(Instrument &&) = default;
  Instrument(Instrument const &) = delete;

  operator Depth const &() const { return depth_; }

  bool is_ready() const { return ready_; }

  auto tick_size() const { return tick_size_; }

  auto min_trade_vol() const { return min_trade_vol_; }

  auto multiplier() const { return multiplier_; }

  auto is_market_open() const { return trading_status_ == TradingStatus::OPEN; }

  auto best_bid() const { return depth_[0].bid_price; }

  auto best_ask() const { return depth_[0].ask_price; }

  double position() const;

  bool can_trade(Side side) const;

  void operator()(Connected const &);
  void operator()(Disconnected const &);
  void operator()(DownloadBegin const &);
  void operator()(DownloadEnd const &);
  void operator()(GatewayStatus const &);
  void operator()(ReferenceData const &);
  void operator()(MarketStatus const &);
  void operator()(MarketByPriceUpdate const &);
  void operator()(OrderUpdate const &);
  void operator()(PositionUpdate const &);

 protected:
  void check_ready();

  void reset();

  void validate(Depth const &);

 private:
  std::string_view const exchange_;
  std::string_view const symbol_;
  std::string_view const account_;
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
