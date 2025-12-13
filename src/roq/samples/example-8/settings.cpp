/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-8/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace example_8 {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
