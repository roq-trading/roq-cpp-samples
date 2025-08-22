/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/samples/experiment/settings.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Controller;

struct Shared final {
  Shared(Settings const &, Controller &);

  Settings const &settings;
  Controller &controller;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
