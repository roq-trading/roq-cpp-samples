/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/bridge/application.hpp"

#include <cassert>
#include <vector>

#include "roq/client.hpp"

#include "roq/samples/bridge/bridge.hpp"
#include "roq/samples/bridge/config.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace bridge {

// === IMPLEMENTATION ===

int Application::main_helper(std::span<std::string_view> const &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  if (std::size(args) != 2)
    log::fatal("Expected exactly one argument"sv);
  Config config;
  auto connections = args.subspan(1);
  // this is where you start the dispatch loop
  client::Bridge{config, connections}.dispatch<Bridge>();
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
