/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/model.h"

#include <numeric>

#include "roq/logging.h"

#include "roq/utils/compare.h"

#include "roq/samples/example-3/flags.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_3 {

Model::Model() : bid_ema_(Flags::ema_alpha()), ask_ema_(Flags::ema_alpha()) {
}

void Model::reset() {
  bid_ema_.reset();
  ask_ema_.reset();
  selling_ = false;
  buying_ = false;
}

Side Model::update(const Depth &depth) {
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
      log::info("SIGNAL: BUY @ {}"_fmt, depth[0].ask_price);
      result = Side::BUY;
      selling_ = false;
    }
  } else {
    if (ask_fast < bid_slow && ask_fast < ask_slow) {
      log::info("DIRECTION: SELLING"_sv);
      selling_ = true;
      buying_ = false;
    }
  }

  if (buying_) {
    if (ready && bid_fast > bid_slow) {
      log::info("SIGNAL: SELL @ {}"_fmt, depth[0].bid_price);
      result = Side::SELL;
      buying_ = false;
    }
  } else {
    if (bid_fast > ask_slow && bid_fast > bid_slow) {
      log::info("DIRECTION: BUYING"_sv);
      buying_ = true;
      selling_ = false;
    }
  }

  // can't be both
  assert(2 != ((selling_ ? 1 : 0) + (buying_ ? 1 : 0)));

  log::trace_1(
      "model={{"
      "bid={} "
      "ask={} "
      "bid_fast={} "
      "ask_fast={} "
      "bid_slow={} "
      "ask_slow={} "
      "selling={} "
      "buying={}"
      "}}"_fmt,
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

bool Model::validate(const Depth &depth) {  // require full depth
  return std::accumulate(depth.begin(), depth.end(), true, [](bool value, const Layer &layer) {
    return value && utils::compare(layer.bid_quantity, 0.0) > 0 && utils::compare(layer.ask_quantity, 0.0) > 0;
  });
}

double Model::weighted_bid(const Depth &depth) {
  double sum_1 = 0.0, sum_2 = 0.0;
  std::for_each(depth.begin(), depth.end(), [&](const Layer &layer) {
    sum_1 += layer.bid_quantity * layer.bid_price;
    sum_2 += layer.bid_quantity;
  });
  return sum_1 / sum_2;
}

double Model::weighted_ask(const Depth &depth) {
  double sum_1 = 0.0, sum_2 = 0.0;
  std::for_each(depth.begin(), depth.end(), [&](const Layer &layer) {
    sum_1 += layer.ask_quantity * layer.ask_price;
    sum_2 += layer.ask_quantity;
  });
  return sum_1 / sum_2;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
