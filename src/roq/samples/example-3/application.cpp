/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-3/application.hpp"

#include <chrono>

#include "roq/algo/matcher/factory.hpp"

#include "roq/algo/reporter/factory.hpp"

#include "roq/samples/example-3/config.hpp"
#include "roq/samples/example-3/settings.hpp"
#include "roq/samples/example-3/strategy.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_3 {

// === CONSTANTS ===

namespace {
auto const SNAPSHOT_FREQUENCY = 1s;
auto const MATCHER = "simple"sv;  // note! filled when market is crossed
auto const MARKET_DATA_LATENCY = 1ms;
auto const ORDER_MANAGEMENT_LATENCY = 10ms;
}  // namespace

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  // if (std::size(params) != 1)
  //   log::fatal("Expected exactly one argument"sv);
  Settings settings{args};
  Config config{settings};

  client::Trader{settings, config, params}.dispatch<Strategy>(settings);

  return EXIT_SUCCESS;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
