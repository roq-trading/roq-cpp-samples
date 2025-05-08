/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/event-log/application.hpp"

#include "roq/logging.hpp"

#include "roq/samples/event-log/processor.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace event_log {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) != 1) {
    log::fatal("Expected exactly 1 argument, got {}"sv, std::size(params));
  }
  Processor{params[0]}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace event_log
}  // namespace samples
}  // namespace roq
