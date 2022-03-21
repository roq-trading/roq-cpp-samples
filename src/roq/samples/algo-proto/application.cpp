/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/algo-proto/application.hpp"

#include <vector>

#include "roq/exceptions.hpp"

#include "roq/samples/algo-proto/bridge.hpp"
#include "roq/samples/algo-proto/config.hpp"

#include "roq/samples/algo-proto/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace algo_proto {

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

int Application::main_helper(const std::span<std::string_view> &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  if (std::size(args) != 2)
    log::fatal("Expected exactly one argument"sv);
  Config config{flags::Flags::config_file()};
  auto connections = args.subspan(1);
  client::Trader(config, connections).dispatch<Bridge>(config, std::size(connections));
  return EXIT_SUCCESS;
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
