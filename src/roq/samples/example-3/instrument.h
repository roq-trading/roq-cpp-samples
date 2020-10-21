/* Copyright (c) 2017-2020, Hans Erik Thrane */

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

  operator const Depth &() const { return _depth; }

  bool is_ready() const { return _ready; }

  auto tick_size() const { return _tick_size; }

  auto min_trade_vol() const { return _min_trade_vol; }

  auto multiplier() const { return _multiplier; }

  auto is_market_open() const { return _trading_status == TradingStatus::OPEN; }

  auto best_bid() const { return _depth[0].bid_price; }

  auto best_ask() const { return _depth[0].ask_price; }

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
  const std::string_view _exchange;
  const std::string_view _symbol;
  const std::string_view _account;
  ConnectionStatus _connection_status = ConnectionStatus::DISCONNECTED;
  bool _download = false;
  double _tick_size = std::numeric_limits<double>::quiet_NaN();
  double _min_trade_vol = std::numeric_limits<double>::quiet_NaN();
  double _multiplier = std::numeric_limits<double>::quiet_NaN();
  TradingStatus _trading_status = TradingStatus::UNDEFINED;
  GatewayStatus _market_data_status = GatewayStatus::DISCONNECTED;
  GatewayStatus _order_manager_status = GatewayStatus::DISCONNECTED;
  Depth _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  double _long_position = 0.0;
  double _short_position = 0.0;
  bool _ready = false;
  uint32_t _last_order_id = 0;
  double _last_traded_quantity = 0.0;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
