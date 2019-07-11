/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/application.h"

#include "roq/samples/common/config.h"

#include "roq/samples/taker/strategy.h"

DEFINE_bool(simulation, false, "Simulation");
DEFINE_string(generator_type, "csv", "Generator type");
DEFINE_string(generator_name, "CFFEX", "Generator name");
DEFINE_string(matcher_type, "simple", "Matcher type");
DEFINE_uint32(matcher_buffer_size, (64 + 1) * 4096, "Matcher buffer size");
DEFINE_uint32(market_data_latency, 0, "Market data latency");
DEFINE_uint32(order_manager_latency, 0, "Order manager latency");
DEFINE_string(results_directory, "", "Results directory");
DEFINE_string(results_format, "", "Results format (csv|json)");

namespace {
constexpr const char *DESCRIPTION = "Taker Strategy (Roq Samples)";
}  // namespace

namespace roq {
namespace samples {
namespace taker {

class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    common::Config config {
      .instruments = { {
        .exchange = "CFFEX",
        .symbol = "IC1906",
        .net_limit = 1,
        .tick_size = 0.01,
        .multiplier = 100.0,
        .accounts = { {
          "A1", {
            .long_limit = 10,
            .short_limit = 10,
            .long_start_of_day = 0.0,
            .short_start_of_day = 0.0,
          }
        } },
      }, {
        .exchange = "CFFEX",
        .symbol = "IF1906",
        .net_limit = 0,
        .tick_size = 0.01,
        .multiplier = 100.0,
        .accounts = {}
      }, {
        .exchange = "CFFEX",
        .symbol = "IH1906",
        .net_limit = 0,
        .tick_size = 0.01,
        .multiplier = 100.0,
        .accounts = {},
      } }
    };
    std::vector<std::string> arguments(argv + 1, argv + argc);
    create_and_dispatch(config, arguments);
    return EXIT_SUCCESS;
  }

  template <typename T, typename... Args>
  void create_and_dispatch(
      const common::Config& config,
      const T& arguments,
      Args&&... args) {
    if (FLAGS_simulation) {
      simulate(
          config,
          arguments,
          std::forward<Args>(args)...);
    } else {
      trade(
          config,
          arguments,
          std::forward<Args>(args)...);
    }
  }

  template <typename T, typename... Args>
  void simulate(
      const common::Config& config,
      const T& arguments,
      Args&&... args) {
    using roq::client::detail::SimulationFactory;
    // parse options
    std::chrono::milliseconds market_data_latency {
      FLAGS_market_data_latency };
    std::chrono::milliseconds order_manager_latency {
        FLAGS_order_manager_latency };
    // create the event generator
    auto generator = SimulationFactory::create_generator(
        FLAGS_generator_type,
        arguments);
    // create the matcher
    auto matcher = SimulationFactory::create_matcher(
        FLAGS_matcher_type,
        FLAGS_generator_name,
        market_data_latency,
        order_manager_latency,
        FLAGS_matcher_buffer_size);
    // create the collector
    auto collector = client::detail::SimulationFactory::create_collector(
        std::chrono::milliseconds(1));
    // create the strategy and dispatch
    roq::client::Simulator(config, *generator, *matcher, *collector)
      .dispatch<Strategy>(
          config,
          std::forward<Args>(args)...);
    // export collector results
    collector->write(FLAGS_results_directory, FLAGS_results_format);
  }

  template <typename T, typename... Args>
  void trade(
      const common::Config& config,
      const T& arguments,
      Args&&... args) {
    // create and dispatch
    roq::client::Trader(config, arguments)
      .dispatch<Strategy>(
          config,
          std::forward<Args>(args)...);
  }
};

}  // namespace taker
}  // namespace samples
}  // namespace roq

int main(int argc, char **argv) {
  return roq::samples::taker::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
