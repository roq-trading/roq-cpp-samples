/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/import/application.hpp"

#include <stdexcept>
#include <vector>

#include "roq/exceptions.hpp"

#include "roq/samples/import/processor.hpp"
#include "roq/samples/import/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace import {

// === IMPLEMENTATION ===

int Application::main_helper(std::span<std::string_view> const &args) {
  if (std::size(args) != 2)
    log::fatal("Expected exactly 1 argument, got {}"sv, std::size(args) - 1);
  Settings settings;
  Processor{settings, args[1]}.dispatch();
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace import
}  // namespace samples
}  // namespace roq
