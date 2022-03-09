/* Copyright (c) 2017-2022, Hans Erik Thrane */

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

int Application::main_helper(const std::span<std::string_view> &args) {
  assert(!std::empty(args));
  if (std::size(args) != 3)
    log::fatal("Expected exactly two arguments"sv);
  Config config;
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto snapshot_frequency = 1s;
    auto collector = client::detail::SimulationFactory::create_collector(snapshot_frequency);
    // simulator
    auto create_generator = [&connections](auto source_id) {
      return client::detail::SimulationFactory::create_generator(connections[source_id], source_id);
    };
    auto create_matcher = [](auto &dispatcher) {
      return client::detail::SimulationFactory::create_matcher(dispatcher, "simple"sv);
    };
    client::Simulator::Factory factories[] = {
        {
            .create_generator = create_generator,
            .create_matcher = create_matcher,
            .market_data_latency = 1ms,
            .order_management_latency = 5ms,
        },
        {
            .create_generator = create_generator,
            .create_matcher = create_matcher,
            .market_data_latency = 100ms,
            .order_management_latency = 150ms,
        },
    };
    client::Simulator(config, factories).dispatch<Strategy>();
  } else {
    // trader
    client::Trader(config, connections).dispatch<Strategy>();
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
