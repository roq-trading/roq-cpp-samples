/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/application.hpp"

#include <vector>

#include "roq/exceptions.hpp"

#include "roq/io/engine/libevent/context_factory.hpp"

#include "roq/samples/io-context/config.hpp"
#include "roq/samples/io-context/controller.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

int Application::main_helper(std::span<std::string_view> const &args, io::Context &context) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments should be a list of unix domain sockets
  auto connections = args.subspan(1);
  // note!
  //   using client::Bridge so we can dispatch events through the Timer event
  client::Bridge(config, connections).dispatch<Controller>(context);
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  // wrap arguments (prefer to not work with raw pointers)
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  // note!
  //   create a specific instance of io::Context -- libevent
  auto libevent = io::engine::libevent::ContextFactory::create();
  return main_helper(args, *libevent);
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
