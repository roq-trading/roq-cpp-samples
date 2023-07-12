/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/example-3/flags/flags.hpp"

namespace roq {
namespace samples {
namespace example_3 {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
