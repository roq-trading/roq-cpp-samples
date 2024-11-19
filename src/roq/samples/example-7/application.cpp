/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-7/application.hpp"

#include <cassert>

#include "roq/samples/example-7/config.hpp"
#include "roq/samples/example-7/settings.hpp"
#include "roq/samples/example-7/strategy.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_7 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  if (std::size(params) < 1)
    log::fatal("Expected at least one argument"sv);
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
