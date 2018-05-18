/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <roq/filesystem.h>
#include <roq/logging.h>

#include "common/config.h"

#include "config/config_reader.h"
#include "config/key_value.h"

DEFINE_string(strategy, "", "strategy config file (path).");
DEFINE_string(strategy_name, "", "strategy name.");
DEFINE_string(strategy_group, "", "strategy group config file (path).");

using namespace examples;  // NOLINT
using namespace examples::config;  // NOLINT

namespace {
static common::Config convert(const Strategy& strategy) {
  std::vector<common::Config::Instrument> instruments;
  bool first = true;
  for (const auto& symbol : strategy.instruments) {
    std::map<std::string, common::Config::Instrument::Account> accounts;
    if (first) {
      for (const auto& account : strategy.accounts) {
        accounts[account] = common::Config::Instrument::Account {
          .long_limit = 0,  // TODO(thraneh): need a parser for tradelimit
          .short_limit = 0,
          .long_start_of_day = std::numeric_limits<double>::quiet_NaN(),
          .short_start_of_day = std::numeric_limits<double>::quiet_NaN()
        };
      }
    }
    auto instrument = common::Config::Instrument {
      .exchange = "CFFEX",
      .symbol = symbol,
      .tick_size = 0.02,
      .multiplier = 100.0,
      std::move(accounts)};
    instruments.emplace_back(std::move(instrument));
    first = false;
  }
  return common::Config {
    .instruments = std::move(instruments)
  };
}
}  // namespace

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
    auto strategy = ConfigReader::parse(
        FLAGS_strategy_name,
        FLAGS_strategy_group);
    LOG(INFO) << strategy;
    // TODO(thraneh): clean up
    auto directory = roq::filesystem::dirname(FLAGS_strategy_group);
    auto config_file = directory + "/" + strategy.config_file;
    LOG(INFO) << config_file;
    KeyValue config(config_file);
    LOG(INFO) << config;
  }

  return EXIT_SUCCESS;
}
