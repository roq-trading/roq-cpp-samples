/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "config/json.h"

#include <roq/logging.h>

namespace examples {
namespace config {

JSON::JSON(const std::string& path) {
  LOG(FATAL) << "Not implemented";
}

std::ostream& JSON::write(std::ostream& stream) const {
  return stream;
}

}  // namespace config
}  // namespace examples
