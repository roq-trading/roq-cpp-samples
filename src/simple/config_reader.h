/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>

#include "simple/config.h"

namespace examples {
namespace simple {

class ConfigReader final {
 public:
  static Config read(
      const std::string& config_file,
      const std::string& config_variables);
};

}  // namespace simple
}  // namespace examples
