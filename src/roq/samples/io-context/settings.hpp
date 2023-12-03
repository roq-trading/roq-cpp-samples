/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/samples/io-context/flags/flags.hpp"

namespace roq {
namespace samples {
namespace io_context {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
