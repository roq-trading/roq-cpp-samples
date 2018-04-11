/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <map>
#include <string>

namespace examples {
namespace common {

class ConfigVariables final {
 public:
  static std::map<std::string, std::string> read(
      const std::string& config_variables);
};

}  // namespace common
}  // namespace examples
