/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <string>

namespace examples {
namespace simple {

struct Config final {
  std::string exchange;
  std::string symbol;
  double tick_size;
  bool weighted;
  double threshold;
  double quantity;
};

std::ostream& operator<<(std::ostream& stream, const Config& value);

}  // namespace simple
}  // namespace examples
