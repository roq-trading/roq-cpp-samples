/* Copyright (c) 2017-2021, Hans Erik Thrane */

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
  EMA bid_ema_;
  EMA ask_ema_;
  bool selling_ = false;
  bool buying_ = false;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
