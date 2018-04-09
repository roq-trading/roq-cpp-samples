/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/strategy.h"

#include <roq/logging.h>

namespace examples {
namespace simple {

Strategy::Strategy(
    roq::Strategy::Dispatcher& dispatcher,
    const Config& config,
    const std::string& gateway)
    : BaseStrategy(
          dispatcher,
          config.exchange,
          config.instrument,
          gateway),
      _weighted(config.weighted),
      _threshold(config.threshold),
      _quantity(config.quantity) {
}

void Strategy::reset() {
  _previous = std::numeric_limits<double>::quiet_NaN();
}

void Strategy::update(const MarketData& market_data) {
  const auto& best = market_data.depth[0];
  // do we have a proper two-sided market?
  if (best.bid_quantity == 0.0 && best.ask_quantity == 0.0)
    return;
  // compute the signal and compare to previous
  auto value = compute(market_data);
  auto signal = value - _previous;
  _previous = value;
  // CSV output (note std::cout is being used instead of logger)
  std::cout <<
    "S," <<  // prefix to help with filtering
    market_data.exchange_time.time_since_epoch().count() << "," <<
    best.bid_price << "," <<
    best.bid_quantity << "," <<
    best.ask_price << "," <<
    best.ask_quantity << "," <<
    value << "," <<
    signal <<
    std::endl;
  // only trade if the magnitude of the signal exceeds the threshold
  if (std::fabs(signal) < _threshold)
    return;
  // direction of the signal
  auto sign_signal = (0.0 < signal) - (signal < 0.0);  // stack overflow: 1903954
  // direction of current position
  auto position = get_position();
  auto sign_position = (0.0 < position) - (position < 0.0);  // stack overflow: 1903954
  // exposure is limited to configured quantity
  // in other words: do not increase an already existing position
  if ((sign_signal * sign_position) < 0)
    return;
  // all checks have passed: create the order
  try {
    switch (sign_signal) {
      case 1:
        try_trade(roq::TradeDirection::Sell, _quantity, best.bid_price);
        break;
      case -1:
        try_trade(roq::TradeDirection::Buy, _quantity, best.ask_price);
        break;
    }
  } catch (roq::Exception& e) {
    // possible reasons;
    // roq::NotConnected
    // - client has lost connection to gateway
    // roq::NotReady
    // - gateway has lost connection to broker
    // - gateway is downloading information to client
    LOG(WARNING) << "Failed to create order. Reason=\"" << e.what() << "\"";
  }
}

double Strategy::compute(const MarketData& market_data) const {
  if (_weighted) {
    // weighted mid price
    // a real-life application would probably choose to assign
    // importance (weighting) based on layer's distance from best
    double sum_1 = 0.0, sum_2 = 0.0;
    for (const auto& layer : market_data.depth) {
      sum_1 += layer.bid_price * layer.bid_quantity +
               layer.ask_price * layer.ask_quantity;
      sum_2 += layer.bid_quantity + layer.ask_quantity;
    }
    return sum_1 / sum_2 / get_tick_size();
  } else {
    // simple mid price
    const auto& best = market_data.depth[0];
    return 0.5 * (best.bid_price + best.ask_price) / get_tick_size();
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
