/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-8/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/example-8/config.hpp"
#include "roq/samples/example-8/settings.hpp"
#include "roq/samples/example-8/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
