/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/strategy.h"

#include <roq/logging.h>
#include <roq/stream.h>

namespace examples {
namespace simple {

namespace {
// Constants
const char *PREFIX_SIGNAL = "S";
const char *PREFIX_CREATE_ORDER = "O";
const char *DELIMITER = ",";
const char *QUOTE = "\"";
// Compute sign (stack overflow: 1903954)
template <typename T>
int sign(T value) {
  return (T(0) < value) - (value < T(0));
}
}  // namespace

Strategy::Strategy(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& gateway,
    const Config& config)
    : common::SimpleStrategy(
          dispatcher,
          gateway,
          config.account,
          config.exchange,
          config.symbol,
          config.tick_size),
      _weighted(config.weighted),
      _threshold(config.threshold),
      _quantity(config.quantity) {
}

void Strategy::update(const common::MarketData& market_data) {
  const auto& best = market_data.depth[0];
  // Is it a proper two-sided market?
  if (best.bid_quantity == 0.0 && best.ask_quantity == 0.0)
    return;
  // Compute signal and compare to previous.
  auto value = compute(
      market_data.depth,
      sizeof(market_data.depth) / sizeof(market_data.depth[0]));
  auto signal = value - _previous;
  _previous = value;
  // Write signal to std::cout [csv].
  write_signal(market_data.exchange_time, best, value, signal);
  // Only trade if the magnitude of the signal exceeds threshold
  if (std::fabs(signal) < _threshold || std::isnan(signal))
    return;
  // Direction of signal.
  auto sign_signal = sign(signal);
  // Direction of current position.
  auto position = get_net_position(common::PositionType::Current);
  auto sign_position = sign(position);
  // Exposure is limited to configured quantity.
  // In other words: do not increase an already existing position.
  if ((sign_signal * sign_position) < 0)
    return;
  // All pre-trade checks have now passed.
  // Find arguments for the create_order function...
  auto args = create_order_args(sign_signal, best);
  // ... so the arguments can be written to std::cout [csv]
  write_create_order(market_data.exchange_time, args);
  // ... and then call the create_order function with those same arguments
  try {
    // (c++17's std::apply would be very convenient here :-)
    create_order(
        std::get<0>(args),   // side
        std::get<1>(args),   // quantity
        std::get<2>(args),   // price
        std::get<3>(args));  // order template
  } catch (roq::Exception& e) {
    // Possible reasons;
    //   roq::NotConnected
    //   - Client has lost connection to gateway
    //   roq::NotReady
    //   - Gateway is not in the ready state (e.g. disconnected from
    //     broker or downloading information to client)
    //   - Instrument status doesn't allow trading (e.g. not currently
    //     a trading session or trading halt)
    //   - Any other validating checks performed by the base strategy
    // You can avoid the roq::NotReady exception by checking the is_ready()
    // method before trying to create a new order.
    // You must, however, always be prepared to deal with disconnection
    // between gateway and client.
    LOG(WARNING) << "Unable to create order. Reason=\"" << e.what() << "\"";
  }
}

double Strategy::compute(const roq::Layer *depth, size_t length) const {
  if (_weighted) {
    // Weighted mid price.
    // A real-life application should probably assign
    // importance (weighting) based on layer's distance
    // from best.
    double sum_1 = 0.0, sum_2 = 0.0;
    for (auto i = 0; i < length; ++i) {
      const auto& layer = depth[i];
      sum_1 += layer.bid_price * layer.bid_quantity +
               layer.ask_price * layer.ask_quantity;
      sum_2 += layer.bid_quantity + layer.ask_quantity;
    }
    return sum_1 / sum_2 / get_tick_size();
  } else {
    // Simple mid price.
    const auto& best = depth[0];
    return 0.5 * (best.bid_price + best.ask_price) / get_tick_size();
  }
}

Strategy::create_order_args_t Strategy::create_order_args(
    int sign_signal,
    const roq::Layer& best) const {
  switch (sign_signal) {
    case 1: {  // sell on the up-tick
      auto close =
          get_short_position(common::PositionType::NewActivity) <
          get_long_position(common::PositionType::StartOfDay);
      return std::make_tuple(
          roq::Side::Sell,
          _quantity,
          best.bid_price,
          get_order_template(close));
    }
    case -1: {  // buy on the down-tick
      auto close =
          get_long_position(common::PositionType::NewActivity) <
          get_short_position(common::PositionType::StartOfDay);
      return std::make_tuple(
          roq::Side::Buy,
          _quantity,
          best.ask_price,
          get_order_template(close));
    }
    default: {
      LOG(FATAL) << "Unexpected (sign_signal=" << sign_signal << ")";
    }
  }
}

// CSV output

void Strategy::write_signal(
    roq::time_point_t exchange_time,
    const roq::Layer& best,
    double value,
    double signal) {
  auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(
    exchange_time.time_since_epoch()).count();
  std::cout <<
    PREFIX_SIGNAL << DELIMITER <<
    (msecs / 1000) << DELIMITER <<
    (msecs % 1000) << DELIMITER <<
    best.bid_price << DELIMITER <<
    best.bid_quantity << DELIMITER <<
    best.ask_price << DELIMITER <<
    best.ask_quantity << DELIMITER <<
    value << DELIMITER <<
    signal <<
    std::endl;
}

void Strategy::write_create_order(
    roq::time_point_t exchange_time,
    const Strategy::create_order_args_t& args) {
  auto msecs = std::chrono::duration_cast<std::chrono::milliseconds>(
    exchange_time.time_since_epoch()).count();
  std::cout <<
    PREFIX_CREATE_ORDER << DELIMITER <<
    (msecs / 1000) << DELIMITER <<
    (msecs % 1000) << DELIMITER <<
    std::get<0>(args) << DELIMITER <<       // side
    std::get<1>(args) << DELIMITER <<       // quantity
    std::get<2>(args) << DELIMITER <<       // price
    QUOTE << std::get<3>(args) << QUOTE <<  // order template
    std::endl;
}

}  // namespace simple
}  // namespace examples
