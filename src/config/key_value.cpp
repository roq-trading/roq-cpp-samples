/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/key_value.h"

#include <roq/logging.h>

#include <fstream>
#include <map>

namespace examples {
namespace config {

namespace {
static ucl::Ucl parse(const std::string& path) {
  std::ifstream file(path);
  LOG_IF(FATAL, file.fail()) << "Unable to read \"" << path << "\"";
  std::string data((std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());
  std::string error;
  auto ucl = ucl::Ucl::parse(data, error);
  LOG_IF(FATAL, !error.empty()) << error;
  return ucl;
}
}  // namespace

KeyValue::KeyValue(const std::string& path)
    : _ucl(parse(path)) {
}

bool KeyValue::exists(const std::string& key) const {
  return _ucl.lookup(key).type() != UCL_NULL;
}

bool KeyValue::get_bool(const std::string& key) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_BOOLEAN:
      return ucl.bool_value();
    case UCL_NULL:
      throw std::runtime_error("Missing");
    default:
      throw std::runtime_error("Unexpected type");
  }
}

bool KeyValue::get_bool(
    const std::string& key,
    bool default_value) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_BOOLEAN:
      return ucl.bool_value();
    case UCL_NULL:
      return default_value;
    default:
      throw std::runtime_error("Unexpected type");
  }
}

int64_t KeyValue::get_int(const std::string& key) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_INT:
      return ucl.int_value();
    case UCL_NULL:
      throw std::runtime_error("Missing");
    default:
      throw std::runtime_error("Unexpected type");
  }
}

int64_t KeyValue::get_int(
    const std::string& key,
    int64_t default_value) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_INT:
      return ucl.int_value();
    case UCL_FLOAT:
      return static_cast<int64_t>(ucl.number_value());
    case UCL_NULL:
      return default_value;
    default:
      throw std::runtime_error("Unexpected type");
  }
}

double KeyValue::get_double(const std::string& key) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_INT:
      return static_cast<double>(ucl.int_value());
    case UCL_FLOAT:
      return ucl.number_value();
    case UCL_NULL:
      throw std::runtime_error("Missing");
    default:
      throw std::runtime_error("Unexpected type");
  }
}

double KeyValue::get_double(
    const std::string& key,
    double default_value) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_FLOAT:
      return ucl.number_value();
    case UCL_NULL:
      return default_value;
    default:
      throw std::runtime_error("Unexpected type");
  }
}

std::string KeyValue::get_string(const std::string& key) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_STRING:
      return ucl.string_value();
    case UCL_NULL:
      throw std::runtime_error("Missing");
    default:
      throw std::runtime_error("Unexpected type");
  }
}

std::string KeyValue::get_string(
    const std::string& key,
    const std::string& default_value) const {
  auto ucl = _ucl.lookup(key);
  switch (ucl.type()) {
    case UCL_STRING:
      return ucl.string_value();
    case UCL_NULL:
      return default_value;
    default:
      throw std::runtime_error("Unexpected type");
  }
}

std::ostream& KeyValue::write(std::ostream& stream) const {
  bool first = true;
  for (auto& iter : _ucl) {
    if (!first)
      stream << ", ";
    first = false;
    stream << iter.key() << "=";
    switch (iter.type()) {
      case UCL_OBJECT:
        LOG(FATAL) << "Nested objects not supported (key=\"" << iter.key() << "\"";
        break;
      case UCL_ARRAY:
        LOG(FATAL) << "Array objects not supported (key=\"" << iter.key() << "\"";
        break;
      case UCL_INT:
        stream << iter.int_value();
        break;
      case UCL_FLOAT:
        stream << iter.number_value();
        break;
      case UCL_STRING:
        stream << "\"" << iter.string_value() << "\"";
        break;
      case UCL_BOOLEAN:
        stream << (iter.bool_value() ? "true" : "false");
        break;
      case UCL_TIME:
        LOG(FATAL) << "Time objects not supported (key=\"" << iter.key() << "\"";
      case UCL_USERDATA:
        LOG(FATAL) << "Userdata objects not supported (key=\"" << iter.key() << "\"";
      case UCL_NULL:
        break;
    }
  }
  return stream;
}

}  // namespace config
}  // namespace examples
