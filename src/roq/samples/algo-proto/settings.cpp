/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/algo-proto/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
