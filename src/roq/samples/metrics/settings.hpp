/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/metrics/flags/flags.hpp"

namespace roq {
namespace samples {
namespace metrics {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace metrics
}  // namespace samples
}  // namespace roq
