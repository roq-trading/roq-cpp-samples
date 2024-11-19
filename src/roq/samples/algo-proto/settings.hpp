/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/algo-proto/flags/flags.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
