/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-8/application.hpp"

#include <cassert>
#include <vector>

#include "roq/exceptions.hpp"

#include "roq/samples/example-8/config.hpp"
#include "roq/samples/example-8/settings.hpp"
#include "roq/samples/example-8/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_8 {

// === IMPLEMENTATION ===

int Application::main_helper(std::span<std::string_view> const &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  Settings settings;
  Config config{settings};
  auto connections = args.subspan(1);
  client::Trader{config, connections}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace example_8
}  // namespace samples
}  // namespace roq
