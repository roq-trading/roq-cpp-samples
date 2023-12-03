/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-3/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>

#include "roq/samples/example-3/config.hpp"
#include "roq/samples/example-3/settings.hpp"
#include "roq/samples/example-3/strategy.hpp"

using namespace std::chrono_literals;
using namespace std::literals;

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
  if (std::size(params) != 1)
    log::fatal("Expected exactly one argument"sv);
  Settings settings{args};
  Config config{settings};
  // note!
  //   params can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
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
    auto factory = client::Simulator::Factory{
        .create_generator = create_generator,
        .create_matcher = create_matcher,
        .market_data_latency = MARKET_DATA_LATENCY,
        .order_management_latency = ORDER_MANAGEMENT_LATENCY,
    };
    client::Simulator{settings, config, factory, *collector}.dispatch<Strategy>(settings);
  } else {
    // trader
    client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  }
  return EXIT_SUCCESS;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
