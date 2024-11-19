/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/metrics/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace metrics {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace metrics
}  // namespace samples
}  // namespace roq
