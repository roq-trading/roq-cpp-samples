/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/client.h>
#include <roq/simulation.h>
#include <roq/logging.h>

#include "collector/collector.h"
#include "utilities/generator.h"

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=user:password@:/var/tmp/femasapi.sock\".");

DEFINE_string(simulation_file, "",
    "Simulation file (path)");

using namespace examples::collector;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library
  roq::logging::Logger::initialize(true);

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_gateways.empty()) {
    // mode: simulation

    if (FLAGS_simulation_file.empty()) {
      LOG(ERROR) << "Missing parameter: --simulation-file";
      std::exit(EXIT_FAILURE);
    }

    std::list<std::unique_ptr<roq::simulation::Generator> > generators;
    generators.emplace_back(
        new examples::utilities::Generator(FLAGS_simulation_file));

    roq::simulation::Controller<Collector>(
        std::move(generators)).create_and_dispatch();

  } else {
    // mode: trading

    if (FLAGS_simulation_file.empty() == false) {
      LOG(ERROR) << "Not possible to combine simulation with live trading.";
      std::exit(EXIT_FAILURE);
    }

    auto gateways = roq::client::Gateways::create(FLAGS_gateways);

    roq::client::Controller<Collector>(
        std::move(gateways)).create_and_dispatch();
  }

  return EXIT_SUCCESS;
}
