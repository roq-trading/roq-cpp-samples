/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/bridge/application.hpp"

#include "roq/logging.hpp"

#include "roq/client.hpp"

#include "roq/samples/bridge/bridge.hpp"
#include "roq/samples/bridge/config.hpp"
#include "roq/samples/bridge/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace bridge {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  if (std::size(params) != 1) {
    log::fatal("Expected exactly one argument"sv);
  }
  Settings settings{args};
  Config config{settings};
  // this is where you start the dispatch loop
  client::Bridge{settings, config, params}.dispatch<Bridge>();
  return EXIT_SUCCESS;
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
