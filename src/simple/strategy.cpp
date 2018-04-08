/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/strategy.h"

#include <roq/logging.h>

namespace examples {
namespace simple {

void Strategy::reset() {
  _previous = std::numeric_limits<double>::quiet_NaN();
}

void Strategy::update(const MarketData& market_data) {
  const auto& best = market_data.depth[0];
  // do we have a proper two-sided market?
  if (best.bid_quantity == 0.0 && best.ask_quantity == 0.0)
    return;
  // mid price computation and compare to previous
  auto mid_price = 0.5 * (best.bid_price + best.ask_price);
  auto change_in_ticks = (mid_price - _previous) / get_tick_size();
  _previous = mid_price;
  // do we have a jump?
  if (std::fabs(change_in_ticks) < _threshold)
    return;
  // branch-free sign (stack overflow: 1903954)
  auto sign_change = (0.0 < change_in_ticks) - (change_in_ticks < 0.0);
  auto position = get_position();
  auto sign_position = (0.0 < position) - (position < 0.0);
  // are we going to increase an already existing position?
  if ((sign_change * sign_position) < 0)
    return;
  // try to send a new order
  try {
    switch (sign_change) {
      case 1: {
        try_trade(roq::TradeDirection::Sell, _quantity, best.bid_price);
        break;
      }
      case -1: {
        try_trade(roq::TradeDirection::Buy, _quantity, best.ask_price);
        break;
      }
    }
  } catch (roq::Exception& e) {
    LOG(WARNING) << "Failed to create order. Reason=\"" << e.what() << "\"";
  }
}

void Strategy::try_trade(
    roq::TradeDirection direction,
    double quantity,
    double price) {
  // first we close yesterday's position, then we open new positions
  switch (direction) {
    case roq::TradeDirection::Buy: {
      if (get_long_position(PositionType::NewActivity) <
          get_short_position(PositionType::StartOfDay))
        buy_ioc_close(quantity, price);
      else
        buy_ioc_open(quantity, price);
      break;
    }
    case roq::TradeDirection::Sell: {
      if (get_short_position(PositionType::NewActivity) <
          get_long_position(PositionType::StartOfDay))
        sell_ioc_close(quantity, price);
      else
        sell_ioc_open(quantity, price);
      break;
    }
  }
}

}  // namespace simple
}  // namespace examples
