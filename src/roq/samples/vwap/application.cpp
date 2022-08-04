/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/vwap/application.hpp"

#include <stdexcept>
#include <vector>

#include "roq/exceptions.hpp"

#include "roq/samples/vwap/processor.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace vwap {

int Application::main_helper(std::span<std::string_view> const &args) {
  if (std::size(args) < 2)
    log::fatal("Expected at least 1 argument"sv);
  for (auto &path : args.subspan(1))
    Processor::dispatch(path);
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

}  // namespace vwap
}  // namespace samples
}  // namespace roq
