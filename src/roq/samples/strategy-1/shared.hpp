/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/samples/strategy-1/settings.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

struct Controller;

struct Shared final {
  Shared(Settings const &, Controller &);

  Settings const &settings;
  Controller &controller;
};

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
