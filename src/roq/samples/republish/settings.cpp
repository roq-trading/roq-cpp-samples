/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/republish/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace republish {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
