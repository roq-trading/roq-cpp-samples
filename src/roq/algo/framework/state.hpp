/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <span>
#include <string>
#include <vector>

#include "roq/cache/gateway.hpp"

#include "roq/algo/framework/instrument.hpp"
#include "roq/algo/framework/market.hpp"

namespace roq {
namespace algo {
namespace framework {

struct State final {
  State(
      std::span<Instrument const> const &, std::span<Market const *> const &, std::span<cache::Gateway const> const &);

  State(State &&) = default;
  State(State const &) = default;

  size_t size() const { return std::size(instruments_); }

  Instrument const &get_instrument(size_t index) const { return instruments_[index]; }
  Market const &get_market(size_t index) const { return *markets_[index]; }

  template <typename Callback>
  void dispatch(Callback callback) const {
    for (auto &instrument : instruments_)
      callback(instrument);
  }

  bool ready(bool &result, size_t index, uint8_t source, std::string_view const &account) const;

  bool supports(size_t index, SupportType const &) const;

 private:
  std::string const type_;
  std::vector<Instrument> const instruments_;
  std::vector<Market const *> const markets_;
  std::span<cache::Gateway const> const gateways_;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
