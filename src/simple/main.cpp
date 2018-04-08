/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/client.h>
#include <roq/logging.h>

#include "simple/config_reader.h"
#include "simple/strategy.h"

DEFINE_string(config_file, "",
    "Config filename for strategy (path)");
DEFINE_string(config_variables, "",
    "Optional config filename for variables (path)");

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=user:password@:/var/tmp/femasapi.sock\".");

using namespace examples::simple;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library
  roq::logging::Logger::initialize(true);

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    std::exit(EXIT_FAILURE);
  }

  auto config = ConfigReader::parse(
      FLAGS_config_file,
      FLAGS_config_variables);

  if (FLAGS_gateways.empty()) {
    LOG(ERROR) << "Missing parameter: --gateways";
    std::exit(EXIT_FAILURE);
  }

  auto gateways = roq::client::Gateways::create(FLAGS_gateways);

  LOG_IF(FATAL, gateways.size() != 1) << "Expected exactly one gateway";

  const auto& gateway = (*gateways.begin()).first;

  roq::client::Controller<Strategy>(
      std::move(gateways)).create_and_dispatch(
          config, gateway);

  return EXIT_SUCCESS;
}
