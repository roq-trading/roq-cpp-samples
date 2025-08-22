/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/strategy-1/application.hpp"

#include "roq/logging.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/samples/strategy-1/config.hpp"
#include "roq/samples/strategy-1/controller.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace strategy_1 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  Settings settings{args};
  Config config{settings};
  auto context = io::engine::ContextFactory::create(settings.engine);
  Controller{settings, config, *context, params}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace strategy_1
}  // namespace samples
}  // namespace roq
