/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <string_view>

#include "roq/api.hpp"

#include "roq/cache/gateway.hpp"
#include "roq/cache/market.hpp"

namespace roq {
namespace algo {
namespace framework {

struct Instrument final {
  Instrument() = default;

  bool ready(cache::Gateway const &, cache::Market const &, std::string_view const &account) const;

  Exchange exchange;
  Symbol symbol;
  double weight = NaN;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
