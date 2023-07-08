/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/algo-proto/settings.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
