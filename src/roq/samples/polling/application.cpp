/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/polling/application.hpp"

#include "roq/logging.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/samples/polling/config.hpp"
#include "roq/samples/polling/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace polling {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  Settings settings{args};
  Config config{settings};
  auto context = io::engine::ContextFactory::create(settings.engine);
  Strategy{settings, config, *context, params}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace polling
}  // namespace samples
}  // namespace roq
