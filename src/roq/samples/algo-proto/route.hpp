/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_set.h>

#include <string_view>

#include "roq/algo/framework/market.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Route final {
  Route(uint32_t market_id, const std::string_view &exchange, const std::string_view &symbol);

  Route(Route &&) = default;
  Route(const Route &) = delete;

  uint8_t get_source() const { return market_.get_source(); }

  operator const algo::framework::Market &() const { return market_; }

  template <typename T, typename Callback>
  bool dispatch(const Event<T> &event, Callback callback) {
    if (!market_(event))
      return false;
    for (auto strategy_id : strategies_)
      callback(strategy_id);
    return true;
  }

  void add(uint32_t strategy_id) { strategies_.emplace(strategy_id); }
  void remove(uint32_t strategy_id) { strategies_.erase(strategy_id); }

 private:
  algo::framework::Market market_;
  absl::flat_hash_set<uint32_t> strategies_;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
