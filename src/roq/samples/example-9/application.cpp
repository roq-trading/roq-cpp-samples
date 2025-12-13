/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-9/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/example-9/config.hpp"
#include "roq/samples/example-9/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_9 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) != 1) {
    log::fatal("Expected exactly one argument"sv);
  }
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

}  // namespace example_9
}  // namespace samples
}  // namespace roq
