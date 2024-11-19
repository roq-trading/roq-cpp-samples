/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/depth/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace depth {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace depth
}  // namespace samples
}  // namespace roq
