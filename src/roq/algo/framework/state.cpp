/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/algo/framework/state.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace framework {

State::State(
    std::span<Instrument const> const &instruments,
    std::span<Market const *> const &markets,
    std::span<const cache::Gateway> const &gateways)
    : instruments_(std::begin(instruments), std::end(instruments)), markets_(std::begin(markets), std::end(markets)),
      gateways_(gateways) {
  assert(std::size(instruments_) == std::size(markets_));
}

bool State::ready(bool &result, size_t index, uint8_t source, std::string_view const &account) const {
  auto &market = *markets_[index];
  if (market.get_source() != source)
    return false;
  auto &instrument = instruments_[index];
  return utils::update(result, instrument.ready(gateways_[source], market, account));
}

bool State::supports(size_t index, SupportType const &type) const {
  auto &market = *markets_[index];
  auto source = market.get_source();
  if (source == SOURCE_SELF)
    return false;
  return gateways_[source](type);
}

}  // namespace framework
}  // namespace algo
}  // namespace roq
