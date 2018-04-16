/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/config_reader.h"

#include <roq/logging.h>

#include <rapidjson/document.h>

#include <fstream>
#include <vector>

namespace examples {
namespace config {

namespace {
static rapidjson::Document parse_json(const std::string& path) {
  std::ifstream file(path);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << path << "\"";
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  rapidjson::Document document;
  document.Parse(data.c_str());
  if (document.HasParseError())
    LOG(FATAL) << "Unable to parse config file";
  return document;
}
std::vector<std::string> parse_array(
    const rapidjson::GenericValue<rapidjson::UTF8<> >& value) {
  if (value.IsArray() == false)
    LOG(FATAL) << "Unexpected";
  std::vector<std::string> result;
  for (auto i = 0; i < value.Size(); ++i)
    result.emplace_back(value[i].GetString());
  return result;
}
}  // namespace

Strategy ConfigReader::parse(
    const std::string& strategy_name,
    const std::string& config_file) {
  auto document = parse_json(config_file);
  const auto& strategy_list = document["StrategyList"];
  LOG_IF(FATAL, strategy_list.IsArray() == false) << "Unexpected";
  bool found = false;
  for (auto i = 0; i < strategy_list.Size(); ++i) {
    const auto& strategy = strategy_list[i];
    if (strategy_name != strategy["StrategyName"].GetString())
      continue;
    return Strategy {
      .strategy_kind = strategy["StrategyKind"].GetString(),
      .instruments = parse_array(strategy["InstIdList"]),
      .accounts = parse_array(strategy["AccountIdList"]),
      .config_file = strategy["ConfigFileName"].GetString(),
    };
  }
  throw std::runtime_error("Strategy not found");
}

}  // namespace config
}  // namespace examples
