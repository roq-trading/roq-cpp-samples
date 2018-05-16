/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "spread/config_reader.h"

#include <ucl++.h>

#include <roq/logging.h>

#include <fstream>
#include <limits>
#include <map>

#include "common/config_variables.h"

namespace examples {
namespace spread {

namespace {
// FIXME(thraneh): drop hard-coded config
const auto NaN = std::numeric_limits<double>::quiet_NaN();
const common::Config CONFIG = {
  .instruments = {
    {
      .exchange = "CFFEX",
      .symbol = "T1806",
      .tick_size = 0.01,
      .multiplier = 1.0,
      .accounts = {
        { "A1", { 10.0, 10.0, NaN, NAN } },
      },
    },
    {
      .exchange = "CFFEX",
      .symbol = "TF1806",
      .tick_size = 0.01,
      .multiplier = 1.0,
      .accounts = {
        { "A1", { 20.0, 20.0, NaN, NaN } },
        { "B2", { 20.0, 20.0, NaN, NaN } },
      },
    },
  },
};
}  // namespace


namespace {
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
  Config result {
    .config = {
      .instruments = CONFIG.instruments
    },
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

}  // namespace spread
}  // namespace examples
