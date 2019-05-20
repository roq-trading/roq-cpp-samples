/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/application.h"

#include "roq/samples/taker/strategy.h"

DEFINE_bool(simulation, false, "Simulation");
DEFINE_string(generator_type, "csv", "Generator type");
DEFINE_string(generator_name, "CFFEX", "Generator name");
DEFINE_string(matcher_type, "simple", "Matcher type");
DEFINE_uint32(matcher_buffer_size, (64 + 1) * 4096, "Matcher buffer size");
DEFINE_uint32(market_data_latency, 0, "Market data latency");
DEFINE_uint32(order_manager_latency, 0, "Order manager latency");

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
    std::vector<std::string> arguments(argv + 1, argv + argc);
    common::Config config {};  // TODO(thraneh): parse config
    create_and_dispatch(arguments, config);
    return EXIT_SUCCESS;
  }

  template <typename T, typename... Args>
  void create_and_dispatch(const T& arguments, Args&&... args) {
    if (FLAGS_simulation) {
      simulate(arguments, std::forward<Args>(args)...);
    } else {
      trade(arguments, std::forward<Args>(args)...);
    }
  }

  template <typename T, typename... Args>
  void simulate(const T& arguments, Args&&... args) {
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
    roq::client::Simulator(*generator, *matcher, *collector)
      .dispatch<Strategy>(std::forward<Args>(args)...);
    // TODO(thraneh): command-line arguments to control collector output
  }

  template <typename T, typename... Args>
  void trade(const T& arguments, Args&&... args) {
    // create and dispatch
    roq::client::Trader(arguments)
      .dispatch<Strategy>(std::forward<Args>(args)...);
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
