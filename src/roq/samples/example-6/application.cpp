/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-6/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>

#include "roq/samples/example-6/config.hpp"
#include "roq/samples/example-6/settings.hpp"
#include "roq/samples/example-6/strategy.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_6 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) != 2)
    log::fatal("Expected exactly two arguments"sv);
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>();
  return EXIT_SUCCESS;
}

}  // namespace example_6
}  // namespace samples
}  // namespace roq
