/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/logging.h>

#include "config/json.h"
#include "config/key_value.h"

DEFINE_string(strategy, "", "strategy (path).");
DEFINE_string(strategy_group, "", "strategy group (path).");

using namespace examples::config;  // NOLINT

int main(int argc, char *argv[]) {
  // Initialize logging library.
  roq::logging::Logger::initialize(true);

  // Parse command-line options.

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Strategy?

  if (FLAGS_strategy.empty() == false) {
    KeyValue config(FLAGS_strategy);
    LOG(INFO) << config;
    LOG(INFO) << config.get_string("Instrument");
    LOG(INFO) << config.get_double("p18");
  }

  if (FLAGS_strategy_group.empty() == false) {
    JSON config(FLAGS_strategy_group);
    LOG(INFO) << config;
  }

  return EXIT_SUCCESS;
}
