/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-3/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

#include "roq/client.hpp"
#include "roq/exceptions.hpp"

#include "roq/samples/example-3/config.hpp"
#include "roq/samples/example-3/flags.hpp"
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

int Application::main_helper(std::span<std::string_view> const &args) {
  assert(!std::empty(args));
  if (std::size(args) == 1)
    log::fatal("Expected arguments"sv);
  if (std::size(args) != 2)
    log::fatal("Expected exactly one argument"sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
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
    client::Simulator::Factory factory{
        .create_generator = create_generator,
        .create_matcher = create_matcher,
        .market_data_latency = MARKET_DATA_LATENCY,
        .order_management_latency = ORDER_MANAGEMENT_LATENCY,
    };
    client::Simulator{config, factory, *collector}.dispatch<Strategy>();
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

}  // namespace example_3
}  // namespace samples
}  // namespace roq
