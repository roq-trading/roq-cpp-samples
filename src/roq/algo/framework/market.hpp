/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/cache/market.hpp"

namespace roq {
namespace algo {
namespace framework {

// note! intercept source
struct Market final {
  template <typename MarketByPriceFactory>
  Market(uint32_t market_id, std::string_view const &exchange, std::string_view const &symbol, MarketByPriceFactory create_market_by_price)
      : market_(market_id, exchange, symbol, create_market_by_price) {}

  Market(Market &&) = default;
  Market(Market const &) = delete;

  uint8_t get_source() const { return source_; }

  operator cache::Market const &() const { return market_; }

  template <typename T>
  bool operator()(Event<T> const &event) {
    if (source_ == SOURCE_SELF)
      source_ = event.message_info.source;
    return market_(event);
  }

 private:
  cache::Market market_;
  uint8_t source_ = SOURCE_SELF;  // must be discovered
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
