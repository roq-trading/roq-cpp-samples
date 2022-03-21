/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <span>
#include <string>
#include <vector>

#include "roq/cache/gateway.hpp"

#include "roq/algo/framework/instrument.hpp"
#include "roq/algo/framework/market.hpp"

namespace roq {
namespace algo {
namespace framework {

class State final {
 public:
  State(
      const std::span<const Instrument> &, const std::span<const Market *> &, const std::span<const cache::Gateway> &);

  State(State &&) = default;
  State(const State &) = default;

  size_t size() const { return std::size(instruments_); }

  const Instrument &get_instrument(size_t index) const { return instruments_[index]; }
  const Market &get_market(size_t index) const { return *markets_[index]; }

  template <typename Callback>
  void dispatch(Callback callback) const {
    for (auto &instrument : instruments_)
      callback(instrument);
  }

  bool ready(bool &result, size_t index, uint8_t source, const std::string_view &account) const;

  bool supports(size_t index, const SupportType &) const;

 private:
  const std::string type_;
  const std::vector<Instrument> instruments_;
  const std::vector<const Market *> markets_;
  const std::span<const cache::Gateway> gateways_;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
