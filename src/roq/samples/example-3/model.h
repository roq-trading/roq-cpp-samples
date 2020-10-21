/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <array>

#include "roq/api.h"

#include "roq/samples/example-3/ema.h"

namespace roq {
namespace samples {
namespace example_3 {

class Model final {
 public:
  static constexpr size_t MAX_DEPTH = 3;
  using Depth = std::array<Layer, MAX_DEPTH>;

  Model();

  Model(const Model &) = delete;
  Model(Model &&) = default;

  void reset();

  Side update(const Depth &depth);

 protected:
  bool validate(const Depth &depth);

  double weighted_bid(const Depth &depth);

  double weighted_ask(const Depth &depth);

 private:
  EMA _bid_ema;
  EMA _ask_ema;
  bool _selling = false;
  bool _buying = false;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
