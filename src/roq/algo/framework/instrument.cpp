/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/algo/framework/instrument.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace framework {

// === CONSTANTS ===

namespace {
auto const REQUIRED_MARKET_DATA = Mask{
    SupportType::REFERENCE_DATA,
    SupportType::MARKET_STATUS,
    SupportType::MARKET_BY_PRICE,
};

auto const REQUIRED_ORDER_MANAGEMENT = Mask{
    SupportType::CREATE_ORDER,
    SupportType::CANCEL_ORDER,
};
}  // namespace

// === IMPLEMENTATION ===

bool Instrument::ready(
    cache::Gateway const &gateway, cache::Market const &market, std::string_view const &account) const {
  if (!gateway.ready(REQUIRED_MARKET_DATA))
    return false;
  if (utils::is_zero(market.reference_data.tick_size))
    return false;
  // XXX this might not be the right place...
  if (market.market_status.trading_status != TradingStatus::OPEN)
    return false;
  if (!gateway.ready(REQUIRED_ORDER_MANAGEMENT, account))
    return false;
  return true;
}

}  // namespace framework
}  // namespace algo
}  // namespace roq
