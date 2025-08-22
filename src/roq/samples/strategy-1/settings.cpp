/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/strategy-1/settings.hpp"

#include "roq/client/flags/settings.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
}

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
