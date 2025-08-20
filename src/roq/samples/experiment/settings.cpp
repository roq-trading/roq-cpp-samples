/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/experiment/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace experiment {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace experiment
}  // namespace samples
}  // namespace roq
