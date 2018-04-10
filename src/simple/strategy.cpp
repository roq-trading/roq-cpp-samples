/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

namespace examples {
namespace simple {

namespace {
// constants
const char *PREFIX_SIGNAL = "S";
const char *PREFIX_CREATE_ORDER = "O";
const char *DELIMITER = ",";
const char *QUOTE = "\"";
// sign (stack overflow: 1903954)
template <typename T>
int sign(T value) {
  return (T(0) < value) - (value < T(0));
}
}  // namespace

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
  // write signal to std::cout [csv]
  write_signal(market_data, value, signal);
  // only trade if the magnitude of the signal exceeds the threshold
  if (std::fabs(signal) < _threshold || std::isnan(signal))
    return;
  // direction of signal
  auto sign_signal = sign(signal);
  // direction of current position
  auto position = get_position();
  auto sign_position = sign(position);
  // exposure is limited to configured quantity
  // in other words: do not increase an already existing position
  if ((sign_signal * sign_position) < 0)
    return;
  // all pre-trade checks have now passed
  // arguments for the create_order function...
  auto args = create_order_args(sign_signal, best);
  // ... so the order parameters can be written to std::cout [csv]
  write_create_order(market_data, args);
  // ... and then call the create_order function with those same arguments
  try {
    create_order(
        std::get<0>(args),
        std::get<1>(args),
        std::get<2>(args),
        std::get<3>(args));
  } catch (roq::Exception& e) {
    // possible reasons;
    //   roq::NotConnected
    //   - client has lost connection to gateway
    //   roq::NotReady
    //   - gateway is not in the ready state (e.g. disconnected from
    //     broker or downloading information to client)
    //   - instrument status doesn't allow trading (e.g. not currently
    //     a trading session or trading halt)
    //   - any other validating checks performed by the base strategy
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

Strategy::create_order_args_t Strategy::create_order_args(
    int sign_signal,
    const roq::Layer& best) const {
  switch (sign_signal) {
    case 1: {
      auto close =
          get_short_position(PositionType::NewActivity) <
          get_long_position(PositionType::StartOfDay);
      return std::make_tuple(
          roq::TradeDirection::Sell,
          _quantity,
          best.bid_price,
          close ? _ioc_close : _ioc_open);
    }
    case -1: {
      auto close =
          get_long_position(PositionType::NewActivity) <
          get_short_position(PositionType::StartOfDay);
      return std::make_tuple(
          roq::TradeDirection::Buy,
          _quantity,
          best.ask_price,
          close ? _ioc_close : _ioc_open);
    }
    default: {
      LOG(FATAL) << "Unexpected (sign_signal=" << sign_signal << ")";
    }
  }
}

void Strategy::write_signal(
    const MarketData& market_data,
    double value,
    double signal) {
  const auto& best = market_data.depth[0];
  std::cout <<
    PREFIX_SIGNAL << DELIMITER <<
    market_data.exchange_time.time_since_epoch().count() << DELIMITER <<
    best.bid_price << DELIMITER <<
    best.bid_quantity << DELIMITER <<
    best.ask_price << DELIMITER <<
    best.ask_quantity << DELIMITER <<
    value << DELIMITER <<
    signal <<
    std::endl;
}

void Strategy::write_create_order(
    const MarketData& market_data,
    const Strategy::create_order_args_t& args) {
  std::cout <<
    PREFIX_CREATE_ORDER << DELIMITER <<
    market_data.exchange_time.time_since_epoch().count() << DELIMITER <<
    std::get<0>(args) << DELIMITER <<       // trade direction
    std::get<1>(args) << DELIMITER <<       // quantity
    std::get<2>(args) << DELIMITER <<       // price
    QUOTE << std::get<3>(args) << QUOTE <<  // order template
    std::endl;
}

}  // namespace simple
}  // namespace examples
