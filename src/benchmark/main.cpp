/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <benchmark/benchmark.h>

#include <roq/logging.h>

#include "common/base_strategy.h"

namespace {
class LogManager {
 public:
  LogManager() {
    roq::logging::Logger::initialize(false);
  }
  virtual ~LogManager() {
    roq::logging::Logger::shutdown();
  }
};
class BenchmarkDispatcher final : public roq::Client::Dispatcher {
 public:
  void send(
      const roq::CreateOrder& create_order,
      const std::string& gateway) override {
  }
  void send(
      const roq::ModifyOrder& modify_order,
      const std::string& gateway) override {
  }
  void send(
      const roq::CancelOrder& cancel_order,
      const std::string& gateway) override {
  }
};
class BenchmarkStrategy final :
  public LogManager,
  public examples::common::BaseStrategy {
 public:
  BenchmarkStrategy(
      roq::Client::Dispatcher& dispatcher,
      const std::string& gateway,
      const examples::common::Config& config)
      : examples::common::BaseStrategy(dispatcher, gateway, config) {
  }

  void test_create_order() {
    at(0).buy_ioc(1, 100.0);
  }

 protected:
  void update(std::chrono::system_clock::time_point now) override {}
  void update(const examples::common::MarketData& market_data) override {}
};
}  // namespace

class BenchmarkStrategyFixture : public ::benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State&) override {
    auto& client = static_cast<roq::Client&>(_client);
    const auto& subscriptions = client.get_subscriptions();
    // download per gateway ...
    for (const auto& iter : subscriptions) {
      const auto& gateway = iter.first;
      const auto& subscription = iter.second;
      // ... reference data
      download_begin_event(gateway, "");
      for (const auto& symbols : subscription.symbols_by_exchange) {
        const auto& exchange = symbols.first;
        for (const auto& symbol : symbols.second)
          market_status(gateway, exchange, symbol);
      }
      download_end_event(gateway, "");
      // ... accounts (order manager)
      for (const auto& account : subscription.accounts) {
        order_manager_status(gateway, account);
      }
      // ... market data
      market_data_status(gateway);
    }
  }
  void TearDown(const ::benchmark::State&) override {
  }

 private:
  void download_begin_event(
      const std::string& gateway,
      const std::string& account) {
    auto message_info = roq::MessageInfo {
      .source = gateway.c_str(),
    };
    auto download_begin = roq::DownloadBegin {
      .account = account.c_str(),
    };
    auto event = roq::DownloadBeginEvent {
      .message_info = message_info,
      .download_begin = download_begin
    };
    static_cast<roq::Client&>(_client).on(event);
  }
  void download_end_event(
      const std::string& gateway,
      const std::string& account) {
    auto message_info = roq::MessageInfo {
      .source = gateway.c_str(),
    };
    auto download_end = roq::DownloadEnd {
      .account = account.c_str(),
    };
    auto event = roq::DownloadEndEvent {
      .message_info = message_info,
      .download_end = download_end
    };
    static_cast<roq::Client&>(_client).on(event);
  }
  void market_data_status(
      const std::string& gateway) {
    auto message_info = roq::MessageInfo {
      .source = gateway.c_str(),
    };
    auto market_data_status = roq::MarketDataStatus {
      .status = roq::GatewayStatus::Ready,
    };
    auto event = roq::MarketDataStatusEvent {
      .message_info = message_info,
      .market_data_status = market_data_status
    };
    static_cast<roq::Client&>(_client).on(event);
  }
  void order_manager_status(
      const std::string& gateway,
      const std::string& account) {
    auto message_info = roq::MessageInfo {
      .source = gateway.c_str(),
    };
    auto order_manager_status = roq::OrderManagerStatus {
      .account = account.c_str(),
      .status = roq::GatewayStatus::Ready,
    };
    auto event = roq::OrderManagerStatusEvent {
      .message_info = message_info,
      .order_manager_status = order_manager_status
    };
    static_cast<roq::Client&>(_client).on(event);
  }
  void market_status(
      const std::string& gateway,
      const std::string& exchange,
      const std::string& symbol) {
    auto message_info = roq::MessageInfo {
      .source = gateway.c_str(),
    };
    auto market_status = roq::MarketStatus {
      .exchange = exchange.c_str(),
      .symbol = symbol.c_str(),
      .trading_status = roq::TradingStatus::Open,
    };
    auto event = roq::MarketStatusEvent {
      .message_info = message_info,
      .market_status = market_status
    };
    static_cast<roq::Client&>(_client).on(event);
  }

 protected:
  const std::string _gateway = "femas";
  const std::string _exchange = "CFFEX";
  const std::string _symbol = "IC1806";
  const std::string _account = "12345678";
  BenchmarkDispatcher _dispatcher;
  examples::common::Config _config = {
    .instruments = {
      examples::common::Config::Instrument {
        .exchange = _exchange.c_str(),
        .symbol = _symbol.c_str(),
        .tick_size = 0.1,
        .multiplier = 100.0,
        .net_limit = 5,
        .accounts = {
          {
            _account.c_str(),
            examples::common::Config::Instrument::Account {
              .long_limit = 1,
              .short_limit = 1,
              .long_start_of_day = 0,
              .short_start_of_day = 0,
            }
          },
        },
      },
    }
  };
  BenchmarkStrategy _client = {
    _dispatcher,
    _gateway,
    _config
  };
};

BENCHMARK_DEFINE_F(BenchmarkStrategyFixture, BenchmarkStrategyTest)(
    benchmark::State& state) {
  for (auto _ : state) {
    _client.test_create_order();
  }
}
BENCHMARK_REGISTER_F(BenchmarkStrategyFixture, BenchmarkStrategyTest);

BENCHMARK_MAIN();
