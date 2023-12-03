/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-4/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace example_4 {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
