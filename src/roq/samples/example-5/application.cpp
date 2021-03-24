/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-5/application.h"

#include <cassert>
#include <vector>

#include "roq/samples/example-5/config.h"
#include "roq/samples/example-5/strategy.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_5 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(!args.empty());
  if (args.size() == 1u)
    throw std::runtime_error("Expected arguments"_s);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1u);
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

}  // namespace example_5
}  // namespace samples
}  // namespace roq
