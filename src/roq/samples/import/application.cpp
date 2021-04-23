/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/import/application.h"

#include <stdexcept>
#include <vector>

#include "roq/exceptions.h"
#include "roq/literals.h"

#include "roq/samples/import/processor.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace import {

int Application::main_helper(const roq::span<std::string_view> &args) {
  if (args.size() != 2)
    throw RuntimeErrorException("Expected exactly 1 argument, got {}"_fmt, args.size() - 1);
  Processor(args[1]).dispatch();
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
