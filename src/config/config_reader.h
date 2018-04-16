/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include "config/strategy.h"

namespace examples {
namespace config {

class ConfigReader final {
 public:
  static Strategy parse(
      const std::string& strategy_name,
      const std::string& config_file);
};

}  // namespace config
}  // namespace examples
