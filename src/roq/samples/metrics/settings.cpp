/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/metrics/settings.hpp"

namespace roq {
namespace samples {
namespace metrics {

Settings::Settings() : flags::Flags{flags::Flags::create()} {
}

}  // namespace metrics
}  // namespace samples
}  // namespace roq
