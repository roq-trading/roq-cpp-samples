/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/samples/vwap/flags/flags.hpp"

namespace roq {
namespace samples {
namespace vwap {

struct Settings final : public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace vwap
}  // namespace samples
}  // namespace roq
