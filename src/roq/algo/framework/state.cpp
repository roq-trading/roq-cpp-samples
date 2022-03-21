/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/algo/framework/state.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace framework {

State::State(
    const std::span<const Instrument> &instruments,
    const std::span<const Market *> &markets,
    const std::span<const cache::Gateway> &gateways)
    : instruments_(std::begin(instruments), std::end(instruments)), markets_(std::begin(markets), std::end(markets)),
      gateways_(gateways) {
  assert(std::size(instruments_) == std::size(markets_));
}

bool State::ready(bool &result, size_t index, uint8_t source, const std::string_view &account) const {
  auto &market = *markets_[index];
  if (market.get_source() != source)
    return false;
  auto &instrument = instruments_[index];
  return utils::update(result, instrument.ready(gateways_[source], market, account));
}

bool State::supports(size_t index, const SupportType &type) const {
  auto &market = *markets_[index];
  auto source = market.get_source();
  if (source == SOURCE_SELF)
    return false;
  return gateways_[source](type);
}

}  // namespace framework
}  // namespace algo
}  // namespace roq
