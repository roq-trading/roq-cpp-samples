/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <limits>
#include <memory>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// helper class caching instrument specific information

struct Instrument final {
  Instrument(std::string_view const &exchange, std::string_view const &symbol, double alpha);

  Instrument(Instrument &&) = default;
  Instrument(Instrument const &) = delete;

  bool is_ready() const { return ready_; }

  void operator()(Connected const &);
  void operator()(Disconnected const &);
  void operator()(DownloadBegin const &);
  void operator()(DownloadEnd const &);
  void operator()(GatewayStatus const &);
  void operator()(ReferenceData const &);
  void operator()(MarketStatus const &);
  void operator()(MarketByPriceUpdate const &);

 protected:
  void update_model();

  void check_ready();

  void reset();

 private:
  static constexpr size_t MAX_DEPTH = 2;

  std::string_view const exchange_;
  std::string_view const symbol_;
  double const alpha_;

  bool connected_ = false;
  bool download_ = false;
  double tick_size_ = NaN;
  double min_trade_vol_ = NaN;
  double multiplier_ = NaN;
  TradingStatus trading_status_ = {};
  bool market_data_ = {};
  std::array<Layer, MAX_DEPTH> depth_;
  std::unique_ptr<cache::MarketByPrice> market_by_price_;
  double mid_price_ = NaN;
  double avg_price_ = NaN;
  bool ready_ = false;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
