/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/import/application.h"

#include <cassert>
#include <vector>

namespace roq {
namespace samples {
namespace import {

int Application::main_helper(const roq::span<std::string_view> &args) {
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
