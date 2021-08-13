/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-6/application.h"

#include <cassert>
#include <chrono>
#include <memory>
#include <vector>

#include "roq/client.h"
#include "roq/exceptions.h"

#include "roq/samples/example-6/config.h"
#include "roq/samples/example-6/flags.h"
#include "roq/samples/example-6/strategy.h"

using namespace std::chrono_literals;
using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_6 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(!args.empty());
  if (args.size() != 3)
    log::fatal("Expected exactly two arguments"_sv);
  Config config;
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto snapshot_frequency = 1s;
    auto collector = client::detail::SimulationFactory::create_collector(snapshot_frequency);
    // matchers
    std::vector<std::unique_ptr<Matcher> > matchers;
    // ... matcher #1
    auto market_data_latency_1 = 1ms;
    auto order_management_latency_1 = 5ms;
    matchers.emplace_back(client::detail::SimulationFactory::create_matcher(
        "simple"_sv, 0, Flags::source_name_1(), market_data_latency_1, order_management_latency_1));
    // ... matcher #2
    auto market_data_latency_2 = 100ms;
    auto order_management_latency_2 = 150ms;
    matchers.emplace_back(client::detail::SimulationFactory::create_matcher(
        "simple"_sv, 1, Flags::source_name_2(), market_data_latency_2, order_management_latency_2));
    // simulator
    client::Simulator(config, connections, *collector, matchers).dispatch<Strategy>();
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
