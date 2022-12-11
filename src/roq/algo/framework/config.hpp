/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/algo/framework/instrument.hpp"

namespace roq {
namespace algo {
namespace framework {

struct Config final {
  std::string type;
  std::vector<Instrument> instruments;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
