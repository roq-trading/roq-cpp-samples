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
  static const constexpr size_t MAX_DEPTH = 3;

  using Depth = std::array<Layer, MAX_DEPTH>;

  Model();

  Model(Model &&) = default;
  Model(const Model &) = delete;

  void reset();

  Side update(const Depth &);

 protected:
  bool validate(const Depth &);

  double weighted_bid(const Depth &);
  double weighted_ask(const Depth &);

 private:
  EMA bid_ema_;
  EMA ask_ema_;
  bool selling_ = false;
  bool buying_ = false;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
