/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <ucl++.h>

#include <string>

namespace examples {
namespace config {

class KeyValue final {
 public:
  explicit KeyValue(const std::string& path);

  bool exists(const std::string& key) const;

  // bool
  bool get_bool(const std::string& key) const;
  bool get_bool(
      const std::string& key,
      bool default_value) const;

  // int
  int64_t get_int(const std::string& key) const;
  int64_t get_int(
      const std::string& key,
      int64_t default_value) const;

  // double
  double get_double(const std::string& key) const;
  double get_double(
      const std::string& key,
      double default_value) const;

  // string
  std::string get_string(const std::string& key) const;
  std::string get_string(
      const std::string& key,
      const std::string& default_value) const;

  std::ostream& write(std::ostream&) const;

 private:
  ucl::Ucl _ucl;
};

inline std::ostream& operator<<(
    std::ostream& stream,
    const KeyValue& key_value) {
  return key_value.write(stream);
}

}  // namespace config
}  // namespace examples
