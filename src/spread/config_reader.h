/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>

#include "spread/config.h"

namespace examples {
namespace spread {

class ConfigReader final {
 public:
  static Config read(
      const std::string& config_file,
      const std::string& config_variables);
};

}  // namespace spread
}  // namespace examples
