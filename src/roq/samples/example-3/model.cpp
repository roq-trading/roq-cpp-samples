/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/model.h"

#include <numeric>

#include "roq/logging.h"

namespace roq {
namespace samples {
namespace example_3 {

constexpr double TOLERANCE = 1.0e-10;

Model::Model() : _bid_ema(FLAGS_ema_alpha), _ask_ema(FLAGS_ema_alpha) {
}

void Model::reset() {
  _bid_ema.reset();
  _ask_ema.reset();
  _selling = false;
  _buying = false;
}

Side Model::update(const Depth &depth) {
  auto result = Side::UNDEFINED;

  if (validate(depth) == false) return result;

  auto bid_fast = weighted_bid(depth);
  auto bid_slow = _bid_ema.update(bid_fast);
  auto ask_fast = weighted_ask(depth);
  auto ask_slow = _ask_ema.update(ask_fast);

  auto ready = _bid_ema.is_ready() && _ask_ema.is_ready();

  if (_selling) {
    if (ready && ask_fast > ask_slow) {
      LOG(INFO)(R"(SIGNAL: BUY @ {})", depth[0].ask_price);
      result = Side::BUY;
      _selling = false;
    }
  } else {
    if (ask_fast < bid_slow && ask_fast < ask_slow) {
      LOG(INFO)("DIRECTION: SELLING");
      _selling = true;
      _buying = false;
    }
  }

  if (_buying) {
    if (ready && bid_fast > bid_slow) {
      LOG(INFO)(R"(SIGNAL: SELL @ {})", depth[0].bid_price);
      result = Side::SELL;
      _buying = false;
    }
  } else {
    if (bid_fast > ask_slow && bid_fast > bid_slow) {
      LOG(INFO)("DIRECTION: BUYING");
      _buying = true;
      _selling = false;
    }
  }

  // can't be both
  assert(2 != ((_selling ? 1 : 0) + (_buying ? 1 : 0)));

  VLOG(1)
  (R"(model={{)"
   R"(bid={} )"
   R"(ask={} )"
   R"(bid_fast={} )"
   R"(ask_fast={} )"
   R"(bid_slow={} )"
   R"(ask_slow={} )"
   R"(selling={} )"
   R"(buying={})"
   R"(}})",
   depth[0].bid_price,
   depth[0].ask_price,
   bid_fast,
   ask_fast,
   bid_slow,
   ask_slow,
   _selling,
   _buying);

  return result;
}

bool Model::validate(const Depth &depth) {  // require full depth
  return std::accumulate(
      depth.begin(), depth.end(), true, [](bool current, const Layer &layer) {
        return current && std::fabs(layer.bid_quantity) > TOLERANCE &&
               std::fabs(layer.ask_quantity) > TOLERANCE;
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
