/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-7/application.hpp"

#include <cassert>

#include "roq/samples/example-7/collector.hpp"
#include "roq/samples/example-7/config.hpp"
#include "roq/samples/example-7/settings.hpp"
#include "roq/samples/example-7/strategy.hpp"

using namespace std::literals;

using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_7 {

// === CONSTANTS ===

namespace {
auto const MATCHER = "simple"sv;
auto const MARKET_DATA_LATENCY = 1ms;
auto const ORDER_MANAGEMENT_LATENCY = 10ms;
}  // namespace

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  if (std::size(params) < 1)
    log::fatal("Expected at least one argument"sv);
  Settings settings{args};
  Config config{settings};
  if (settings.simulation) {
    // collector
    Collector collector;
    // simulator
    auto create_generator = [&params](auto source_id) {
      return client::detail::SimulationFactory::create_generator(params[source_id], source_id);
    };
    auto create_matcher = [](auto &dispatcher) {
      return client::detail::SimulationFactory::create_matcher(dispatcher, MATCHER);
    };
    client::Simulator::Factory factory{
        .create_generator = create_generator,
        .create_matcher = create_matcher,
        .market_data_latency = MARKET_DATA_LATENCY,
        .order_management_latency = ORDER_MANAGEMENT_LATENCY,
    };
    client::Simulator{settings, config, factory, collector}.dispatch<Strategy>(settings);
  } else {
    // trader
    client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  }
  return EXIT_SUCCESS;
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
