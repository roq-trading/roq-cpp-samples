/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/rest/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace rest {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace rest
}  // namespace samples
}  // namespace roq
