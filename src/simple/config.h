/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>

#include "common/config.h"

namespace examples {
namespace simple {

struct Config final {
  common::Config config;
  bool weighted;
  double threshold;
  double quantity;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);

}  // namespace simple
}  // namespace examples
