/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/bridge/flags/flags.hpp"

namespace roq {
namespace samples {
namespace bridge {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace bridge
}  // namespace samples
}  // namespace roq
