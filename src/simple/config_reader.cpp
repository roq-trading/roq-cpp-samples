/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/config_reader.h"

#include <ucl++.h>

#include <roq/logging.h>

// #include <cstdlib>
#include <fstream>
#include <map>
// #include <utility>
// #include <vector>

namespace examples {
namespace simple {

namespace {
// Parse a variables config file.
static std::map<std::string, std::string> parse_variables(
    const std::string& config_variables) {
  std::map<std::string, std::string> result;
  if (config_variables.empty())
    return result;
  LOG(INFO) << "Parsing variables file (\"" << config_variables << "\")";
  std::ifstream file(config_variables);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << config_variables << "\"";
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
// Parse a config file.
// Optionally expand variables using another config file.
static ucl::Ucl parse_config_file(const std::string& config_file,
    const std::string& config_variables) {
  auto variables = parse_variables(config_variables);
  LOG(INFO) << "Parsing config file (\"" << config_file << "\")";
  std::ifstream file(config_file);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << config_file << "\"";
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  std::string error;
  auto result = ucl::Ucl::parse(data, variables, error);
  LOG_IF(FATAL, !error.empty()) << error;
  return result;
}
// Create config from a parsed config file.
static Config create_config(const ucl::Ucl& setting) {
  Config result {
    .exchange     = setting.lookup("exchange").string_value(),
    .instrument   = setting.lookup("instrument").string_value(),
    .weighted     = setting.lookup("weighted").bool_value(),
    .threshold    = setting.lookup("threshold").number_value(),
    .quantity     = static_cast<double>(
        setting.lookup("quantity").int_value()),
  };
  LOG(INFO) << result;
  return result;
}
}  // namespace

Config ConfigReader::parse(const std::string& config_file,
    const std::string& config_variables) {
  try {
    auto config = parse_config_file(config_file, config_variables);
    auto root = config["strategy"];
    LOG_IF(FATAL, root.type() == UCL_NULL) <<
      "Root (strategy) was not found in the config file";
    return create_config(root);
  } catch (std::exception& e) {
    LOG(FATAL) << e.what();
    std::abort();  // FIXME(thraneh): avoid warning until LOG(FATAL) has been fixed
  }
}

}  // namespace simple
}  // namespace examples
