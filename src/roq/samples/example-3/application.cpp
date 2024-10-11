/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-3/application.hpp"

#include <cassert>
#include <chrono>
#include <memory>

#include "roq/algo/matcher/factory.hpp"

#include "roq/algo/reporter/factory.hpp"

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
}  // namespace

// === HELPERS ===

namespace {
auto create_symbols(auto &settings) {
  using result_type = client::Simulator2::Symbols;
  result_type result;
  result[std::string{settings.exchange}].emplace(settings.symbol);
  return result;
}

auto create_symbols_and_positions(auto &settings) {
  using result_type = client::Simulator2::SymbolsAndPositions;
  result_type result;
  result[std::string{settings.exchange}].emplace(settings.symbol, 0.0);  // note! initial position
  return result;
}

auto create_accounts(auto &settings, auto &symbols_and_positions) {
  using result_type = std::vector<client::Simulator2::Account>;
  result_type result;
  result.emplace_back(settings.account, symbols_and_positions);
  return result;
}

auto create_sources(auto &params, auto &accounts, auto &symbols) {
  using result_type = std::vector<client::Simulator2::Source>;
  result_type result;
  for (auto &item : params) {
    auto source = client::Simulator2::Source{
        .path = item,
        .name = {},
        .order_management{
            .accounts = accounts,
            .latency = ORDER_MANAGEMENT_LATENCY,
        },
        .market_data{
            .symbols = symbols,
            .latency = MARKET_DATA_LATENCY,
        },
    };
    result.emplace_back(std::move(source));
  }
  return result;
}
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
      // !!! OLD SIMULATOR !!!
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
      // !!! NEW SIMULATOR !!!
      struct Factory final : public client::Simulator2::Factory {
        std::unique_ptr<algo::matcher::Handler> create_matcher(
            algo::matcher::Dispatcher &dispatcher,
            algo::OrderCache &order_cache,
            [[maybe_unused]] uint8_t source,
            std::string_view const &exchange,
            std::string_view const &symbol) const override {
          auto config = algo::matcher::Config{
              .exchange = exchange,
              .symbol = symbol,
              .market_data_source = algo::MarketDataSource::MARKET_BY_PRICE,
          };
          return algo::matcher::Factory::create(algo::matcher::Factory::Type::SIMPLE, dispatcher, config, order_cache);
        }
      } factory;

      struct Collector final : public client::Collector {
        void operator()(Event<TradeUpdate> const &event) override { log::warn("event={}"sv, event); }
      } collector;

      auto symbols = create_symbols(settings);
      auto symbols_and_positions = create_symbols_and_positions(settings);
      auto accounts = create_accounts(settings, symbols_and_positions);
      auto sources = create_sources(params, accounts, symbols);

      auto reporter = algo::reporter::Factory::create(algo::reporter::Factory::Type::SUMMARY);

      client::Simulator2{settings, config, factory, *reporter, sources}.dispatch<Strategy>(settings);

      (*reporter).print();
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
