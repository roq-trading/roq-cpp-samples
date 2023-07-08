/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/python/settings.hpp"

namespace roq {
namespace samples {
namespace python {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace python
}  // namespace samples
}  // namespace roq
