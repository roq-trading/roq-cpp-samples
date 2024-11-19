/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/republish/application.hpp"

#include "roq/samples/republish/config.hpp"
#include "roq/samples/republish/settings.hpp"
#include "roq/samples/republish/strategy.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace republish {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  Settings settings{args};
  Config config{settings};
  client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  return EXIT_SUCCESS;
}

}  // namespace republish
}  // namespace samples
}  // namespace roq
