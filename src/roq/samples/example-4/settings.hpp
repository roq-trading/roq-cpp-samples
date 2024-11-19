/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/example-4/flags/flags.hpp"

namespace roq {
namespace samples {
namespace example_4 {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
