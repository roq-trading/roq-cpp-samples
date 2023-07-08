/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/bridge/settings.hpp"

namespace roq {
namespace samples {
namespace bridge {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
