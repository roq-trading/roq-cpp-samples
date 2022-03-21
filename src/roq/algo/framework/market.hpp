/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/cache/market.hpp"

namespace roq {
namespace algo {
namespace framework {

// note! intercept source
class Market final {
 public:
  template <typename MarketByPriceFactory>
  Market(
      uint32_t market_id,
      const std::string_view &exchange,
      const std::string_view &symbol,
      MarketByPriceFactory create_market_by_price)
      : market_(market_id, exchange, symbol, create_market_by_price) {}

  Market(Market &&) = default;
  Market(const Market &) = delete;

  uint8_t get_source() const { return source_; }

  operator const cache::Market &() const { return market_; }

  template <typename T>
  bool operator()(const Event<T> &event) {
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
