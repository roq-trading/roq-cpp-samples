/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <vector>

#include "roq/application.h"
#include "roq/client.h"
#include "roq/logging.h"
#include "roq/format.h"

namespace roq {
namespace samples {
namespace example_1 {

class Config final : public client::Config {
 public:
  Config() {}

 protected:
  void dispatch(Handler& handler) const override {
    // callback for each subscription pattern
    handler.on(
        client::Symbol {
          .exchange = "deribit",
          .regex = "BTC-\\d{2}\\D{3}\\d{2}",  // BTC futures
        });
    handler.on(
        client::Symbol {
          .exchange = "coinbase-pro",
          .regex = "BTC-.*",  // BTC pairs
        });
  }

 private:
  Config(Config&&) = default;
  Config(const Config&) = delete;
  void operator=(const Config&) = delete;
};

class Strategy final : public roq::client::Handler {
 public:
  explicit Strategy(roq::client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher) {
  }

 protected:
  void operator()(const roq::ConnectionStatusEvent& event) override {
    LOG(INFO) << fmt::format(
        "connection_status={}", event);
  }
  void operator()(const roq::DownloadBeginEvent& event) override {
    LOG(INFO) << fmt::format(
        "download_begin={}", event);
  }
  void operator()(const roq::DownloadEndEvent& event) override {
    LOG(INFO) << fmt::format(
        "download_end={}", event);
  }
  void operator()(const roq::MarketDataStatusEvent& event) override {
    LOG(INFO) << fmt::format(
        "market_data_status={}", event);
  }
  void operator()(const roq::OrderManagerStatusEvent& event) override {
    LOG(INFO) << fmt::format(
        "order_manager_status={}", event);
  }
  void operator()(const roq::ReferenceDataEvent& event) override {
    LOG(INFO) << fmt::format(
        "reference_data={}", event);
  }
  void operator()(const roq::MarketStatusEvent& event) override {
    LOG(INFO) << fmt::format(
        "market_status={}", event);
  }
  void operator()(const roq::MarketByPriceEvent& event) override {
    LOG(INFO) << fmt::format(
        "market_by_price={}", event);
  }
  void operator()(const roq::TradeSummaryEvent& event) override {
    LOG(INFO) << fmt::format(
        "trade_summary={}", event);
  }

 private:
  Strategy(Strategy&&) = default;
  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  roq::client::Dispatcher& _dispatcher;
};


class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    roq::client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq

namespace {
constexpr const char *DESCRIPTION = "Example 1 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_1::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
