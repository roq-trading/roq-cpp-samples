/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/json.h"

#include <roq/logging.h>

#include <fstream>
#include <map>
#include <vector>

namespace examples {
namespace config {

namespace {
static rapidjson::Document parse(const std::string& path) {
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
}  // namespace


JSON::JSON(const std::string& path)
    : _document(parse(path)) {
}

namespace {
std::vector<std::string> parse_array(
    const rapidjson::GenericValue<rapidjson::UTF8<> >& value) {
  if (value.IsArray() == false)
    LOG(FATAL) << "Unexpected";
  std::vector<std::string> result;
  for (auto i = 0; i < value.Size(); ++i)
    result.emplace_back(value[i].GetString());
  return result;
}
std::ostream& operator<<(
    std::ostream& stream,
    const std::vector<std::string>& value) {
  stream << "[";
  bool first = true;
  for (const auto& iter : value) {
    if (!first)
      stream << ", ";
    first = false;
    stream << "\"" << iter << "\"";
  }
  return stream << "]";
}
}  // namespace
std::ostream& JSON::write(std::ostream& stream) const {
  const auto& strategy_list = _document["StrategyList"];
  if (strategy_list.IsArray()) {
    bool first = true;
    for (auto i = 0; i < strategy_list.Size(); ++i) {
      const auto& strategy = strategy_list[i];
      const auto& strategy_name = strategy["StrategyName"].GetString();
      const auto& strategy_kind = strategy["StrategyKind"].GetString();
      auto instrument_id_list = parse_array(strategy["InstIdList"]);
      auto account_id_list = parse_array(strategy["AccountIdList"]);
      const auto& config_file_name = strategy["ConfigFileName"].GetString();
      if (first == false)
        stream << ", ";
      first = false;
      stream << "strategy={"
        "strategy_name=\"" << strategy_name << ", "
        "strategy_kind=\"" << strategy_kind << ", "
        "instrument_id_list=" << instrument_id_list <<
        "account_id_list=" << account_id_list <<
        "config_file_name=\"" << config_file_name <<
        "}";
    }
  } else {
    LOG(FATAL) << "Unexpected";
  }
  return stream;
}

}  // namespace config
}  // namespace examples
