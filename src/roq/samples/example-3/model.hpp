/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <array>

#include "roq/api.hpp"

#include "roq/samples/example-3/ema.hpp"

namespace roq {
namespace samples {
namespace example_3 {

class Model final {
 public:
  static constexpr size_t const MAX_DEPTH = 3;

  using Depth = std::array<Layer, MAX_DEPTH>;

  Model();

  Model(Model &&) = default;
  Model(Model const &) = delete;

  void reset();

  Side update(Depth const &);

 protected:
  bool validate(Depth const &);

  double weighted_bid(Depth const &);
  double weighted_ask(Depth const &);

 private:
  EMA bid_ema_;
  EMA ask_ema_;
  bool selling_ = false;
  bool buying_ = false;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
