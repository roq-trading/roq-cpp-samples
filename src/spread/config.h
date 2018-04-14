/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <ostream>

#include "common/config.h"

namespace examples {
namespace spread {

struct Config final {
  common::Config config;
};

std::ostream& operator<<(std::ostream&, const Config&);

}  // namespace spread
}  // namespace examples
