/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <vector>

#include "roq/application.h"
#include "roq/client.h"

#include "roq/logging.h"
#include "roq/format.h"

// command line options

DEFINE_string(deribit_exchange,
    "deribit",
    "name of the deribit exchange");

DEFINE_string(deribit_symbols,
    "BTC-\\d{2}\\D{3}\\d{2}",  // e.g. "BTC-27MAR20"
    "regex used to subscribe deribit symbols");

DEFINE_string(coinbase_pro_exchange,
    "coinbase-pro",
    "name of the coinbase-pro exchange");

DEFINE_string(coinbase_pro_symbols,
    "BTC-.*",  // e.g. "BTC-USD"
    "regex used to subscribe coinbase-pro symbols");

namespace roq {
namespace samples {
namespace example_1 {

// client::Config is an interface allowing a strategy factory
// to query for relevant configuration
// the design uses dispatch to allow the factory to use interfaces
// without imposing any kind of requirement on containers and storage

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config&) = delete;
  Config(Config&&) = default;

  void operator=(const Config&) = delete;
  void operator=(Config&&) = delete;

 protected:
  void dispatch(Handler& handler) const override {
    // callback for each subscription pattern
    handler(
        client::Symbol {
          .regex = FLAGS_deribit_symbols,
          .exchange = FLAGS_deribit_exchange,
        });
    handler(
        client::Symbol {
          .regex = FLAGS_coinbase_pro_symbols,
          .exchange = FLAGS_coinbase_pro_exchange,
        });
  }
};

// client::Handler is the base class managing the life-time of
// a strategy (could be in-process simulation or connectivity
// to one or more market access gateways)
// the constructor must *always* accept a client::Dispatcher as the
// first argument
// the construct *may* accept more arguments from the 2nd position
// event handlers *may* be overriden

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher) {
  }

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

  void operator=(const Strategy&) = delete;
  void operator=(Strategy&&) = delete;

 protected:
  // the following event handlers log every update as-is
  // an important lesson here is to only use verbose logging in
  // the "hot path" (i.e. inside the event handlers)
  // using LOG(INFO) is not disallowed, but you should only use
  // this logging facility very minimally as it may have significant
  // latency impact
  // note!
  //   the ROQ_v environment variable defines the verbosity level
  //   for example, "export ROQ_v=1"
  void operator()(const ConnectionStatusEvent& event) override {
    LOG(INFO)(
        "[{}:{}] ConnectionStatus={}",
        event.message_info.source,
        event.message_info.source_name,
        event.connection_status);
  }
  void operator()(const DownloadBeginEvent& event) override {
    LOG(INFO)(
        "[{}:{}] DownloadBegin={}",
        event.message_info.source,
        event.message_info.source_name,
        event.download_begin);
  }
  void operator()(const DownloadEndEvent& event) override {
    LOG(INFO)(
        "[{}:{}] DownloadEnd={}",
        event.message_info.source,
        event.message_info.source_name,
        event.download_end);
  }
  void operator()(const MarketDataStatusEvent& event) override {
    LOG(INFO)(
        "[{}:{}] MarketDataStatus={}",
        event.message_info.source,
        event.message_info.source_name,
        event.market_data_status);
  }
  void operator()(const OrderManagerStatusEvent& event) override {
    LOG(INFO)(
        "[{}:{}] OrderManagerStatus={}",
        event.message_info.source,
        event.message_info.source_name,
        event.order_manager_status);
  }
  void operator()(const ReferenceDataEvent& event) override {
    LOG(INFO)(
        "[{}:{}] ReferenceData={}",
        event.message_info.source,
        event.message_info.source_name,
        event.reference_data);
  }
  void operator()(const MarketStatusEvent& event) override {
    LOG(INFO)(
        "[{}:{}] MarketStatus={}",
        event.message_info.source,
        event.message_info.source_name,
        event.market_status);
  }
  void operator()(const MarketByPriceEvent& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        "[{}:{}] MarketByPrice={}",
        event.message_info.source,
        event.message_info.source_name,
        event.market_by_price);
  }
  void operator()(const MarketByOrderEvent& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        "[{}:{}] MarketByOrder={}",
        event.message_info.source,
        event.message_info.source_name,
        event.market_by_order);
  }
  void operator()(const TradeSummaryEvent& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        "[{}:{}] TradeSummary={}",
        event.message_info.source,
        event.message_info.source_name,
        event.trade_summary);
  }

 private:
  client::Dispatcher& _dispatcher;
};

// Application is a base class used to initialize the environment
// it is not required, only for your convenience

class Controller final : public Application {
 public:
  using Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    // this strategy factory uses direct connectivity to one or more
    // market access gateways
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq

namespace {
constexpr std::string_view DESCRIPTION = "Example 1 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_1::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
