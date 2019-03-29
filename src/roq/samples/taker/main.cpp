/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/application.h"

#include "roq/samples/taker/strategy.h"

DEFINE_bool(simulation, false, "Simulation");
DEFINE_string(generator_type, "csv", "Generator type");
DEFINE_string(matcher_type, "simple", "Matcher type");
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
    if (FLAGS_simulation) {
      auto generator = roq::client::detail::SimulationFactory::create_generator(
          FLAGS_generator_type,
          arguments);
      roq::client::Simulation<Strategy>(
          std::chrono::nanoseconds(FLAGS_market_data_latency),
          std::chrono::nanoseconds(FLAGS_order_manager_latency),
          config).dispatch(*generator);
    } else {
      roq::client::Trading<Strategy>(
          arguments,
          config).dispatch();
    }
    return EXIT_SUCCESS;
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
