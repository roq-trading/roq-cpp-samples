/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-2/application.hpp"

#include "roq/samples/example-2/config.hpp"
#include "roq/samples/example-2/settings.hpp"
#include "roq/samples/example-2/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_2 {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  if (std::size(params) != 2)
    log::fatal(
        "Expected exactly two arguments: "
        "futures exchange then cash exchange"sv);
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

}  // namespace example_2
}  // namespace samples
}  // namespace roq
