/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/python/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/python/config.hpp"
#include "roq/samples/python/settings.hpp"
#include "roq/samples/python/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace python {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  Settings settings{args};
  Config config{settings};
  // this strategy factory uses direct connectivity to one or more
  // market access gateways
  client::Trader{settings, config, params}.dispatch<Strategy>();
  return EXIT_SUCCESS;
}

}  // namespace python
}  // namespace samples
}  // namespace roq
