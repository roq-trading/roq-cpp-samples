/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-5/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace example_5 {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace example_5
}  // namespace samples
}  // namespace roq
