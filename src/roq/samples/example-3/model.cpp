/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-3/model.hpp"

#include <numeric>

#include "roq/logging.hpp"

#include "roq/utils/compare.hpp"

#include "roq/samples/example-3/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_3 {

// === IMPLEMENTATION ===

Model::Model() : bid_ema_{Flags::ema_alpha()}, ask_ema_{Flags::ema_alpha()} {
}

void Model::reset() {
  bid_ema_.reset();
  ask_ema_.reset();
  selling_ = false;
  buying_ = false;
}

Side Model::update(Depth const &depth) {
  auto result = Side::UNDEFINED;

  if (!validate(depth))
    return result;

  auto bid_fast = weighted_bid(depth);
  auto bid_slow = bid_ema_.update(bid_fast);
  auto ask_fast = weighted_ask(depth);
  auto ask_slow = ask_ema_.update(ask_fast);

  auto ready = bid_ema_.is_ready() && ask_ema_.is_ready();

  if (selling_) {
    if (ready && ask_fast > ask_slow) {
      log::info("SIGNAL: BUY @ {}"sv, depth[0].ask_price);
      result = Side::BUY;
      selling_ = false;
    }
  } else {
    if (ask_fast < bid_slow && ask_fast < ask_slow) {
      log::info("DIRECTION: SELLING"sv);
      selling_ = true;
      buying_ = false;
    }
  }

  if (buying_) {
    if (ready && bid_fast > bid_slow) {
      log::info("SIGNAL: SELL @ {}"sv, depth[0].bid_price);
      result = Side::SELL;
      buying_ = false;
    }
  } else {
    if (bid_fast > ask_slow && bid_fast > bid_slow) {
      log::info("DIRECTION: BUYING"sv);
      buying_ = true;
      selling_ = false;
    }
  }

  // can't be both
  assert(2 != ((selling_ ? 1 : 0) + (buying_ ? 1 : 0)));

  log::info<1>(
      "model={{"
      "bid={} "
      "ask={} "
      "bid_fast={} "
      "ask_fast={} "
      "bid_slow={} "
      "ask_slow={} "
      "selling={} "
      "buying={}"
      "}}"sv,
      depth[0].bid_price,
      depth[0].ask_price,
      bid_fast,
      ask_fast,
      bid_slow,
      ask_slow,
      selling_,
      buying_);

  return result;
}

bool Model::validate(Depth const &depth) {  // require full depth
  return std::accumulate(std::begin(depth), std::end(depth), true, [](bool value, Layer const &layer) {
    return value && utils::is_greater(layer.bid_quantity, 0.0) && utils::is_greater(layer.ask_quantity, 0.0);
  });
}

double Model::weighted_bid(Depth const &depth) {
  double sum_1 = 0.0, sum_2 = 0.0;
  std::for_each(std::begin(depth), std::end(depth), [&](Layer const &layer) {
    sum_1 += layer.bid_quantity * layer.bid_price;
    sum_2 += layer.bid_quantity;
  });
  return sum_1 / sum_2;
}

double Model::weighted_ask(Depth const &depth) {
  double sum_1 = 0.0, sum_2 = 0.0;
  std::for_each(std::begin(depth), std::end(depth), [&](Layer const &layer) {
    sum_1 += layer.ask_quantity * layer.ask_price;
    sum_2 += layer.ask_quantity;
  });
  return sum_1 / sum_2;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
