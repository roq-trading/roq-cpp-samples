/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/depth/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/depth/config.hpp"
#include "roq/samples/depth/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace depth {

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

}  // namespace depth
}  // namespace samples
}  // namespace roq
