/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-4/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/example-4/config.hpp"
#include "roq/samples/example-4/settings.hpp"
#include "roq/samples/example-4/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_4 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>();
  return EXIT_SUCCESS;
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
