/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-2/controller.h"

#include <vector>

#include "roq/samples/example-2/config.h"
#include "roq/samples/example-2/strategy.h"

namespace roq {
namespace samples {
namespace example_2 {

// application

int Controller::main_helper(const roq::span<std::string_view> &args) {
  assert(args.empty() == false);
  if (args.size() == 1)
    throw std::runtime_error("Expected arguments");
  if (args.size() != 3)
    throw std::runtime_error(
        "Expected exactly two arguments: "
        "futures exchange then cash exchange");
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1);
  client::Trader(config, connections).dispatch<Strategy>();
  return EXIT_SUCCESS;
}

int Controller::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
