/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/republish/settings.hpp"

namespace roq {
namespace samples {
namespace republish {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
