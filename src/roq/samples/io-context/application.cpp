/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/io-context/application.hpp"

#include "roq/logging.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/samples/io-context/config.hpp"
#include "roq/samples/io-context/controller.hpp"
#include "roq/samples/io-context/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

// === CONSTANTS ===

namespace {
auto const IO_ENGINE = "libevent"sv;
}

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Expected arguments"sv);
  }
  Settings settings{args};
  Config config{settings};
  // note!
  //   create a specific instance of io::Context
  auto context = io::engine::ContextFactory::create(IO_ENGINE);
  // note!
  //   using client::Bridge so we can dispatch events through the Timer event
  client::Bridge{settings, config, params}.dispatch<Controller>(settings, *context);
  return EXIT_SUCCESS;
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
