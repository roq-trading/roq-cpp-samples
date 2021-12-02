/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-1/application.h"

#include <cassert>
#include <vector>

#include "roq/exceptions.h"

#include "roq/samples/example-1/config.h"
#include "roq/samples/example-1/strategy.h"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_1 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1);
  // this strategy factory uses direct connectivity to one or more
  // market access gateways
  client::Trader(config, connections).dispatch<Strategy>();
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

}  // namespace example_1
}  // namespace samples
}  // namespace roq
