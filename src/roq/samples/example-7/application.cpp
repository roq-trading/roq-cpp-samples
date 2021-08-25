/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-7/application.h"

#include <cassert>
#include <vector>

#include "roq/exceptions.h"

#include "roq/samples/example-7/collector.h"
#include "roq/samples/example-7/config.h"
#include "roq/samples/example-7/flags.h"
#include "roq/samples/example-7/strategy.h"

using namespace roq::literals;

using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_7 {

int Application::main_helper(const roq::span<std::string_view> &args) {
  assert(!args.empty());
  if (args.size() == 1)
    log::fatal("Expected arguments"_sv);
  if (args.size() != 2)
    log::fatal("Expected exactly one argument"_sv);
  Config config;
  // note!
  //   absl::flags will have removed all flags and we're left with arguments
  //   arguments can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
  auto connections = args.subspan(1);
  if (Flags::simulation()) {
    // collector
    auto snapshot_frequency = 1s;
    Collector collector;
    // simulator
    auto create_generator = [&connections](auto source_id) {
      return client::detail::SimulationFactory::create_generator(connections[source_id], source_id);
    };
    auto create_matcher = [](auto &dispatcher) {
      return client::detail::SimulationFactory::create_matcher(dispatcher, "simple"_sv);
    };
    client::Simulator::Factory factory{
        .create_generator = create_generator,
        .create_matcher = create_matcher,
        .market_data_latency = 1ms,
        .order_management_latency = 10ms,
    };
    client::Simulator(config, factory, collector).dispatch<Strategy>();
  } else {
    // trader
    client::Trader(config, connections).dispatch<Strategy>();
  }
  return EXIT_SUCCESS;
}

int Application::main(int argc, char **argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i)
    args.emplace_back(argv[i]);
  return main_helper(args);
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
