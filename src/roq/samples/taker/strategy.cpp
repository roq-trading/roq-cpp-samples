/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/taker/strategy.h"

#include <cmath>

#include "gflags/gflags.h"

#include "roq/api.h"
#include "roq/logging.h"
#include "roq/stream.h"

DEFINE_int32(create_orders, false, "create orders?");

DEFINE_string(
    accounts,
    "A1,A2",
    "accounts, a comma-separated list, e.g. \"A1,A2\"");

DEFINE_string(
    symbols,
    "CFFEX:IC1906,IF1906,IH1906",
    "symbols, one or more comma-separated lists prefixed "
    "with exchange, e.g. \"CFFEX:IC1906,IF1906,IH1906;SHFE:AU1906\")");

DEFINE_bool(real_trading, false, "Real trading?");

// from client
DECLARE_string(name);

namespace roq {
namespace samples {
namespace taker {

namespace {
template <typename T>
int sign(T value) {
  return (T(0) < value) - (value < T(0));
}
constexpr double TOLERANCE = 1.0e-8;
}  // namespace

Strategy::Strategy(
    client::Dispatcher& dispatcher,
    const common::Config& config)
    : common::Strategy(dispatcher, config) {
}

void Strategy::update(std::chrono::nanoseconds now) {
}

void Strategy::update(const CreateOrderAck& create_order_ack) {
}

void Strategy::update(const OrderUpdate& order_update) {
}

void Strategy::update(const TradeUpdate& trade_update) {
}

void Strategy::update(const common::MarketData& market_data) {
  const auto& best = market_data.depth[0];
  // Safety check: is it a proper two-sided market?
  if (std::fabs(best.bid_quantity) < TOLERANCE ||
      std::fabs(best.ask_quantity) < TOLERANCE)
    return;
  // Compute signal and compare to previous computation.
  auto value = compute_mid_price(
      market_data.depth,
      std::size(market_data.depth));
  auto signal = value - _previous;
  _previous = value;
  // Metrics are collected asynchronously: use atomics.
  _signal.store(signal, std::memory_order_relaxed);
  // Only trade if the magnitude of the signal exceeds threshold
  if (std::fabs(signal) < _threshold || std::isnan(signal))
    return;
  // Direction of signal.
  auto sign_signal = sign(signal);
  // Direction of current position.
  auto position = at(0).get_position();
  auto sign_position = sign(position);
  // Exposure is limited to configured quantity.
  // In other words: do not increase an already existing position.
  if ((sign_signal * sign_position) < 0)
    return;
  // All pre-trade checks have now passed.
  if (!FLAGS_real_trading) {
    LOG(WARNING) << "Trading is *not* enabled (use --real-trading)";
    return;
  }
  try {
    switch (sign_signal) {
      case 1: {
        at(0).sell_ioc(_quantity, best.bid_price);
        break;
      }
      case -1: {
        at(0).buy_ioc(_quantity, best.ask_price);
        break;
      }
      default: {
        break;
      }
    }
  } catch (RuntimeError& e) {
    // Possible reasons;
    //   NotConnected
    //   - Client has lost connection to gateway
    //   NotReady
    //   - Gateway is not in the ready state (e.g. disconnected from
    //     broker or downloading information to client)
    //   - Instrument status doesn't allow trading (e.g. not currently
    //     a trading session or trading halt)
    //   - Any other validating checks performed by the base strategy
    // You can avoid the NotReady exception by checking the is_ready()
    // method before trying to create a new order.
    // You must, however, always be prepared to deal with disconnection
    // between gateway and client.
    LOG(WARNING) << "Unable to create order. Reason=\"" << e.what() << "\"";
  }
}

double Strategy::compute_mid_price(
    const Layer *depth,
    size_t length) const {
  if (_weighted) {
    // Weighted mid price.
    // A real-life application should probably assign
    // importance (weighting) based on layer's distance
    // from best.
    double sum_1 = 0.0, sum_2 = 0.0;
    for (size_t i = 0; i < length; ++i) {
      const auto& layer = depth[i];
      sum_1 += layer.bid_price * layer.bid_quantity +
               layer.ask_price * layer.ask_quantity;
      sum_2 += layer.bid_quantity + layer.ask_quantity;
    }
    return sum_1 / sum_2 / at(0).get_tick_size();
  } else {
    // Simple mid price.
    const auto& best = depth[0];
    return 0.5 * (best.bid_price + best.ask_price) / at(0).get_tick_size();
  }
}

void Strategy::write(Metrics& metrics) const {
  metrics
    .write_type("strategy", "gauge")
    .write_simple("strategy", "calc=signal", _signal)
    .finish();
}

}  // namespace taker
}  // namespace samples
}  // namespace roq
