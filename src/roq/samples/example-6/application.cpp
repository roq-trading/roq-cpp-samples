/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-6/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

#include "roq/client.hpp"
#include "roq/exceptions.hpp"

#include "roq/samples/example-6/config.hpp"
#include "roq/samples/example-6/flags.hpp"
#include "roq/samples/example-6/strategy.hpp"

using namespace std::chrono_literals;
using namespace std::literals;

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

int Application::main_helper(std::span<std::string_view> const &args) {
  assert(!std::empty(args));
  if (std::size(args) != 3)
    log::fatal("Expected exactly two arguments"sv);
  Config config;
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto collector = client::detail::SimulationFactory::create_collector(SNAPSHOT_FREQUENCY);
    // simulator
    auto create_generator = [&connections](auto source_id) {
      return client::detail::SimulationFactory::create_generator(connections[source_id], source_id);
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
    client::Simulator{config, factories}.dispatch<Strategy>();
  } else {
    // trader
    client::Trader{config, connections}.dispatch<Strategy>();
  }
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

}  // namespace example_6
}  // namespace samples
}  // namespace roq
