/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-3/application.h"

#include <cassert>
#include <chrono>
#include <vector>

#include "roq/client.h"

#include "roq/samples/example-3/config.h"
#include "roq/samples/example-3/flags.h"
#include "roq/samples/example-3/strategy.h"

using namespace std::literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_3 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(args.empty() == false);
  if (args.size() == 1)
    throw std::runtime_error("Expected arguments"s);
  if (args.size() != 2)
    throw std::runtime_error("Expected exactly one argument"s);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto snapshot_frequency = std::chrono::seconds{1};
    auto collector = client::detail::SimulationFactory::create_collector(snapshot_frequency);
    // matcher
    auto market_data_latency = std::chrono::milliseconds{1};
    auto order_manager_latency = std::chrono::milliseconds{1};
    auto matcher = client::detail::SimulationFactory::create_matcher(
        "simple"sv, Flags::exchange(), market_data_latency, order_manager_latency);
    // simulator
    client::Simulator(config, connections, *collector, *matcher).dispatch<Strategy>();
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

}  // namespace example_3
}  // namespace samples
}  // namespace roq
