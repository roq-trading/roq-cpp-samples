/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/metrics/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/metrics/config.hpp"
#include "roq/samples/metrics/settings.hpp"
#include "roq/samples/metrics/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace metrics {

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

}  // namespace metrics
}  // namespace samples
}  // namespace roq
