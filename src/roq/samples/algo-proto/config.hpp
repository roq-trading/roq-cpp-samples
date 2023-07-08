/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#include "roq/client.hpp"

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
  absl::flat_hash_map<Symbol, algo::framework::Config> strategies;
  absl::flat_hash_map<Exchange, absl::flat_hash_set<Symbol>> exchange_symbols;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
