/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/utils/container.hpp"

#include "roq/algo/framework/market.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Route final {
  Route(uint32_t market_id, std::string_view const &exchange, std::string_view const &symbol);

  Route(Route &&) = default;
  Route(Route const &) = delete;

  uint8_t get_source() const { return market_.get_source(); }

  operator algo::framework::Market const &() const { return market_; }

  template <typename T, typename Callback>
  bool dispatch(Event<T> const &event, Callback callback) {
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
  utils::unordered_set<uint32_t> strategies_;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
