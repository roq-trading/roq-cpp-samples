/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-6/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>

#include "roq/samples/example-6/config.hpp"
#include "roq/samples/example-6/settings.hpp"
#include "roq/samples/example-6/strategy.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_6 {

// === CONSTANTS ===

namespace {
auto const SNAPSHOT_FREQUENCY = 1s;
auto const MATCHER = "simple"sv;
auto const MARKET_DATA_LATENCY_1 = 1ms;
auto const ORDER_MANAGEMENT_LATENCY_1 = 5ms;
auto const MARKET_DATA_LATENCY_2 = 100ms;
auto const ORDER_MANAGEMENT_LATENCY_2 = 150ms;
}  // namespace

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) != 2)
    log::fatal("Expected exactly two arguments"sv);
  Settings settings{args};
  Config config{settings};
  if (settings.simulation) {
    // collector
    auto collector = client::detail::SimulationFactory::create_collector(SNAPSHOT_FREQUENCY);
    // simulator
    auto create_generator = [&params](auto source_id) {
      return client::detail::SimulationFactory::create_generator(params[source_id], source_id);
    };
    auto create_matcher = [](auto &dispatcher) {
      return client::detail::SimulationFactory::create_matcher(dispatcher, MATCHER);
    };
    client::Simulator::Factory factories[] = {
        {
            .create_generator = create_generator,
            .create_matcher = create_matcher,
            .market_data_latency = MARKET_DATA_LATENCY_1,
            .order_management_latency = ORDER_MANAGEMENT_LATENCY_1,
        },
        {
            .create_generator = create_generator,
            .create_matcher = create_matcher,
            .market_data_latency = MARKET_DATA_LATENCY_2,
            .order_management_latency = ORDER_MANAGEMENT_LATENCY_2,
        },
    };
    client::Simulator{settings, config, factories}.dispatch<Strategy>();
  } else {
    // trader
    client::Trader{settings, config, params}.dispatch<Strategy>();
  }
  return EXIT_SUCCESS;
}

}  // namespace example_6
}  // namespace samples
}  // namespace roq
