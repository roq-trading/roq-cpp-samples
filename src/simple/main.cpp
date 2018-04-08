/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/client.h>
#include <roq/logging.h>

#include "simple/strategy.h"

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=user:password@:/var/tmp/femasapi.sock\".");

DEFINE_string(exchange, "", "Exchange name.");
DEFINE_string(instrument, "", "Instrument name.");

DEFINE_string(ioc_open, "ioc_open", "Order template.");
DEFINE_string(ioc_close, "ioc_close", "Order template.");

using namespace examples::simple;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library
  roq::logging::Logger::initialize(true);

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_gateways.empty()) {
    LOG(ERROR) << "Missing parameter: --gateways";
    std::exit(EXIT_FAILURE);
  }

  if (FLAGS_exchange.empty()) {
    LOG(ERROR) << "Missing parameter: --exchange";
    std::exit(EXIT_FAILURE);
  }

  if (FLAGS_instrument.empty()) {
    LOG(ERROR) << "Missing parameter: --instrument";
    std::exit(EXIT_FAILURE);
  }

  auto gateways = roq::client::Gateways::create(FLAGS_gateways);

  LOG_IF(FATAL, gateways.size() != 1) << "Expected exactly one gateway";

  const auto& gateway = (*gateways.begin()).first;

  roq::client::Controller<Strategy>(
      std::move(gateways)).create_and_dispatch(
          FLAGS_exchange, FLAGS_instrument,
          gateway,
          FLAGS_ioc_open, FLAGS_ioc_close);

  return EXIT_SUCCESS;
}
