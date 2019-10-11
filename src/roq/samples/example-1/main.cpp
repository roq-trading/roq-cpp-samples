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

  Config(Config&&) = default;

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
  Config(const Config&) = delete;
  void operator=(const Config&) = delete;
};

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher) {
  }

 protected:
  void operator()(const ConnectionStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const DownloadBeginEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const DownloadEndEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketDataStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const OrderManagerStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const ReferenceDataEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketStatusEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const MarketByPriceEvent& event) override {
    LOG(INFO)("event={}", event);
  }
  void operator()(const TradeSummaryEvent& event) override {
    LOG(INFO)("event={}", event);
  }

 private:
  Strategy(Strategy&&) = default;

  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  client::Dispatcher& _dispatcher;
};


class Controller final : public Application {
 public:
  using Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    client::Trader(config, connections).dispatch<Strategy>();
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
  return roq::samples::example_1::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
