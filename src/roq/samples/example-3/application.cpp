/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-3/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>

#include "roq/algo/matcher/factory.hpp"

#include "roq/samples/example-3/config.hpp"
#include "roq/samples/example-3/settings.hpp"
#include "roq/samples/example-3/strategy.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_3 {

// === CONSTANTS ===

namespace {
auto const SNAPSHOT_FREQUENCY = 1s;
auto const MATCHER = "simple"sv;  // note! filled when market is crossed
auto const MARKET_DATA_LATENCY = 1ms;
auto const ORDER_MANAGEMENT_LATENCY = 10ms;
auto const MATCHER_CONFIG = algo::matcher::Config{
    .source = algo::matcher::Source::TOP_OF_BOOK,
    // .source = algo::matcher::Source::MARKET_BY_PRICE,
};
}  // namespace

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Expected arguments"sv);
  // if (std::size(params) != 1)
  //   log::fatal("Expected exactly one argument"sv);
  Settings settings{args};
  Config config{settings};
  // note!
  //   params can be a list of either
  //   * unix domain sockets (trading) or
  //   * event logs (simulation)
  if (settings.simulation) {
    if (!settings.test_new_simulator) {
      // collector
      auto collector = client::detail::SimulationFactory::create_collector(SNAPSHOT_FREQUENCY);
      // simulator
      auto create_generator = [&params](auto source_id) { return client::detail::SimulationFactory::create_generator(params[source_id], source_id); };
      auto create_matcher = [](auto &dispatcher) { return client::detail::SimulationFactory::create_matcher(dispatcher, MATCHER); };
      auto factory = client::Simulator::Factory{
          .create_generator = create_generator,
          .create_matcher = create_matcher,
          .market_data_latency = MARKET_DATA_LATENCY,
          .order_management_latency = ORDER_MANAGEMENT_LATENCY,
      };
      client::Simulator{settings, config, factory, *collector}.dispatch<Strategy>(settings);
    } else {
      // !!! FOLLOWING IS **EXPERIMENTAL** !!!
      struct Callback final : public client::Simulator2::Callback {
        std::unique_ptr<algo::matcher::Handler> create_matcher(
            algo::matcher::Dispatcher &dispatcher,
            algo::matcher::Cache &cache,
            [[maybe_unused]] uint8_t source_id,
            std::string_view const &exchange,
            std::string_view const &symbol) override {
          return algo::matcher::Factory::create(algo::matcher::Factory::Type::SIMPLE, dispatcher, cache, exchange, symbol, MATCHER_CONFIG);
        }
      } callback;
      std::array<client::Simulator2::Account, 1> accounts{{
          {
              .name = "A1"sv,
              .symbols = {},
          },
      }};
      std::vector<client::Simulator2::Source> sources;
      for (auto &item : params) {
        auto source = client::Simulator2::Source{
            .path = item,
            .order_management =
                {
                    .accounts = accounts,
                    .latency = ORDER_MANAGEMENT_LATENCY,
                },
            .market_data =
                {
                    .symbols = {},
                    .latency = MARKET_DATA_LATENCY,
                },
        };
        log::info("source={}"sv, item);
        sources.emplace_back(std::move(source));
      }
      client::Simulator2{settings, config, callback, sources}.dispatch<Strategy>(settings);
    }
  } else {
    // trader
    client::Trader{settings, config, params}.dispatch<Strategy>(settings);
  }
  return EXIT_SUCCESS;
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
