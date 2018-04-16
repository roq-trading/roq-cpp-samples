/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/json.h"

#include <roq/logging.h>

#include <fstream>
#include <map>

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
  return document;
}
}  // namespace


JSON::JSON(const std::string& path)
    : _document(parse(path)) {
}

std::ostream& JSON::write(std::ostream& stream) const {
  return stream;
}

}  // namespace config
}  // namespace examples
