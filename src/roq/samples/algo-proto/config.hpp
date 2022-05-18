/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#include "roq/client.hpp"

#include "roq/algo/framework/config.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

class Config final : public client::Config {
 public:
  Config(std::string_view const &path);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &handler) const override;

 public:
  absl::flat_hash_map<Symbol, algo::framework::Config> strategies;
  absl::flat_hash_map<Exchange, absl::flat_hash_set<Symbol>> exchange_symbols;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
