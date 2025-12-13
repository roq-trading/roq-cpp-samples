/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/vwap/settings.hpp"

namespace roq {
namespace samples {
namespace vwap {

Settings::Settings(args::Parser const &) : flags::Flags{flags::Flags::create()} {
}

}  // namespace vwap
}  // namespace samples
}  // namespace roq
