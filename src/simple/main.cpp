/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/client.h>
#include <roq/logging.h>

#include "simple/config_reader.h"
#include "simple/strategy.h"

#include "utilities/generator.h"


DEFINE_string(config_file, "",
    "Config filename for strategy (path).");
DEFINE_string(config_variables, "",
    "Optional config filename for variables (path).");

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=user:password@:/var/tmp/femasapi.sock\".");

DEFINE_string(simulation_file, "",
    "Simulation file (path). "
    "For example: \"USTP_Multi_20180223.csv\".");


using namespace examples::simple;  // NOLINT


int main(int argc, char *argv[]) {
  // initialize logging library
  roq::logging::Logger::initialize(true);

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // configuration

  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    std::exit(EXIT_FAILURE);
  }

  auto config = ConfigReader::parse(
      FLAGS_config_file,
      FLAGS_config_variables);

  if (FLAGS_gateways.empty()) {
    // mode: simulation

    if (FLAGS_simulation_file.empty()) {
      LOG(ERROR) << "Missing parameter: --simulation-file";
      std::exit(EXIT_FAILURE);
    }

    // create market-data generators
    std::list<std::unique_ptr<roq::simulation::Generator> > generators;
    generators.emplace_back(
        new examples::utilities::Generator(FLAGS_simulation_file));

    // TODO(thraneh): from matching engine
    std::string gateway("sim");

    roq::simulation::Controller<Strategy>(
        std::move(generators)).create_and_dispatch(
            config, gateway);
  } else {
    // mode: trading

    if (FLAGS_simulation_file.empty() == false) {
      LOG(ERROR) << "Not possible to combine simulation with live trading.";
      std::exit(EXIT_FAILURE);
    }

    // parse gateway connection-details
    auto gateways = roq::client::Gateways::create(FLAGS_gateways);

    LOG_IF(FATAL, gateways.size() != 1) << "Expected exactly one gateway";

    // strategy must know the name of the gateway to send requests to
    const auto& gateway = (*gateways.begin()).first;

    roq::client::Controller<Strategy>(
        std::move(gateways)).create_and_dispatch(
            config, gateway);
  }

  return EXIT_SUCCESS;
}
