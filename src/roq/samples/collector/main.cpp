/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/application.h"

#include "roq/samples/collector/collector.h"
#include "roq/samples/collector/config.h"

// simulation options
DEFINE_bool(simulation, false, "Simulation");
DEFINE_string(generator_type, "csv", "Generator type");
DEFINE_string(generator_name, "CFFEX", "Generator name");


namespace {
constexpr const char *DESCRIPTION = "Collector (Roq Samples)";
}  // namespace

namespace roq {
namespace samples {
namespace collector {

class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    // create collector + start event dispatcher
    create_and_dispatch(
        config,
        connections);
    return EXIT_SUCCESS;
  }

  template <typename T, typename... Args>
  void create_and_dispatch(
      const Config& config,
      const T& connections,
      Args&&... args) {
    if (FLAGS_simulation) {
      simulate(
          config,
          connections,
          std::forward<Args>(args)...);
    } else {
      trade(
          config,
          connections,
          std::forward<Args>(args)...);
    }
  }

  template <typename T, typename... Args>
  void simulate(
      const Config& config,
      const T& connections,
      Args&&... args) {
    using roq::client::detail::SimulationFactory;
    // create the event generator
    auto generator = SimulationFactory::create_generator(
        FLAGS_generator_type,
        connections);
    // create the collector and dispatch
    roq::client::Simulator(config, *generator)
      .dispatch<Collector>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void trade(
      const Config& config,
      const T& connections,
      Args&&... args) {
    // create and dispatch
    roq::client::Trader(config, connections)
      .dispatch<Collector>(std::forward<Args>(args)...);
  }
};

}  // namespace collector
}  // namespace samples
}  // namespace roq

int main(int argc, char **argv) {
  return roq::samples::collector::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
