/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/import/application.hpp"

#include <stdexcept>

#include "roq/samples/import/processor.hpp"
#include "roq/samples/import/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace import {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) != 1)
    log::fatal("Expected exactly 1 argument, got {}"sv, std::size(params));
  Settings settings;
  Processor{settings, params[0]}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace import
}  // namespace samples
}  // namespace roq
