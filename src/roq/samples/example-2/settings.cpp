/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-2/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace example_2 {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
