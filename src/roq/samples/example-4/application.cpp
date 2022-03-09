/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-4/application.hpp"

#include <cassert>
#include <vector>

#include "roq/exceptions.hpp"

#include "roq/samples/example-4/config.hpp"
#include "roq/samples/example-4/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_4 {

int Application::main_helper(const std::span<std::string_view> &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  Config config;
  auto connections = args.subspan(1);
  client::Trader(config, connections).dispatch<Strategy>();
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
