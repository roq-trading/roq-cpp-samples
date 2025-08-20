/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/samples/experiment/settings.hpp"
#include "roq/samples/experiment/test.hpp"

namespace roq {
namespace samples {
namespace experiment {

struct Shared final {
  Shared(Dispatcher &, Settings const &);

  Dispatcher &dispatcher;
  Settings const &settings;
};

}  // namespace experiment
}  // namespace samples
}  // namespace roq
