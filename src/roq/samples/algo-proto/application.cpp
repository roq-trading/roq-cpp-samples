/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/algo-proto/application.hpp"

#include "roq/exceptions.hpp"

#include "roq/samples/algo-proto/bridge.hpp"
#include "roq/samples/algo-proto/config.hpp"

#include "roq/samples/algo-proto/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace algo_proto {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  if (std::size(params) != 1)
    log::fatal("Expected exactly one argument"sv);
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Bridge>(settings, config, std::size(params));
  return EXIT_SUCCESS;
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
