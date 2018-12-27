/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "common/config_variables.h"

#include <ucl++.h>

#include <roq/logging.h>

#include <fstream>

namespace examples {
namespace common {

std::map<std::string, std::string> ConfigVariables::read(
    const std::string& config_variables) {
  std::map<std::string, std::string> result;
  if (config_variables.empty())
    return result;
  LOG(INFO) << "Reading variables file \"" << config_variables << "\"";
  std::ifstream file(config_variables);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << config_variables << "\"";
  LOG(INFO) << "Parsing variables file";
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  std::string error;
  auto config = ucl::Ucl::parse(data, error);
  LOG_IF(FATAL, !error.empty()) << error;
  for (auto iter : config) {
    auto key = iter.key();
    auto value = iter.string_value();
    result.emplace(key, value);
  }
  return result;
}

}  // namespace common
}  // namespace examples
