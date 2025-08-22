/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/strategy-1/flags/flags.hpp"

namespace roq {
namespace samples {
namespace strategy_1 {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
