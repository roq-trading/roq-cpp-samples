/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "simple/config_reader.h"

#include <ucl++.h>

#include <roq/logging.h>

#include <fstream>
#include <limits>
#include <map>

#include "common/config_variables.h"

namespace examples {
namespace simple {

namespace {
const auto NaN = std::numeric_limits<double>::quiet_NaN();
// Read and parse config file.
// Optionally expand variables using another config file.
static ucl::Ucl read_config_file(const std::string& config_file,
    const std::string& config_variables) {
  auto variables = common::ConfigVariables::read(config_variables);
  LOG(INFO) << "Reading config file \"" << config_file << "\"";
  std::ifstream file(config_file);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << config_file << "\"";
  LOG(INFO) << "Parsing config file";
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  std::string error;
  auto result = ucl::Ucl::parse(data, variables, error);
  LOG_IF(FATAL, !error.empty()) << error;
  return result;
}
// Create config object from parsed config file.
static Config create_config(const ucl::Ucl& setting) {
  decltype(common::Config::instruments) instruments;
  // auto portfolio
  Config result {
    .config = {
      .instruments = {
        {
          .exchange = "CFFEX",
          .symbol = "IC1804",
          .accounts = {
            { "A1", { NaN, NAN } },
          },
          .risk_limit = 1.0,
          .tick_size = 0.02,
        },
      },
    },
/*
    .account      = setting.lookup("account").string_value(),
    .exchange     = setting.lookup("exchange").string_value(),
    .symbol       = setting.lookup("symbol").string_value(),
    .tick_size    = setting.lookup("tick_size").number_value(),
*/
    .weighted     = setting.lookup("weighted").bool_value(),
    .threshold    = setting.lookup("threshold").number_value(),
    .quantity     = static_cast<double>(
        setting.lookup("quantity").int_value()),
  };
  LOG(INFO) << result;
  return result;
}
}  // namespace

Config ConfigReader::read(const std::string& config_file,
    const std::string& config_variables) {
  try {
    auto config = read_config_file(config_file, config_variables);
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
