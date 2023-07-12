/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/bridge/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace bridge {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
