/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/utils/container.hpp"

#include "roq/algo/framework/config.hpp"

#include "roq/samples/algo-proto/settings.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;

 public:
  Settings const &settings_;
  utils::unordered_map<std::string, algo::framework::Config> strategies;
  utils::unordered_map<std::string, utils::unordered_set<std::string>> exchange_symbols;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
