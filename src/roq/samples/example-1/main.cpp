/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/logging.h"
#include "roq/service.h"

#include "roq/client.h"

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
  void operator()(const Event<Connection>& event) override {
    LOG(INFO)(
        R"([{}:{}] Connection={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<DownloadBegin>& event) override {
    LOG(INFO)(
        R"([{}:{}] DownloadBegin={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<DownloadEnd>& event) override {
    LOG(INFO)(
        R"([{}:{}] DownloadEnd={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<MarketDataStatus>& event) override {
    LOG(INFO)(
        R"([{}:{}] MarketDataStatus={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<OrderManagerStatus>& event) override {
    LOG(INFO)(
        R"([{}:{}] OrderManagerStatus={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<ReferenceData>& event) override {
    LOG(INFO)(
        R"([{}:{}] ReferenceData={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<MarketStatus>& event) override {
    LOG(INFO)(
        R"([{}:{}] MarketStatus={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<MarketByPriceUpdate>& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        R"([{}:{}] MarketByPriceUpdate={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<MarketByOrderUpdate>& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        R"([{}:{}] MarketByOrderUpdate={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }
  void operator()(const Event<TradeSummary>& event) override {
    // only verbose logging, see comment above
    VLOG(1)(
        R"([{}:{}] TradeSummary={})",
        event.message_info.source,
        event.message_info.source_name,
        event.value);
  }

 private:
  client::Dispatcher& _dispatcher;
};

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

class Controller final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view>& args) {
    assert(args.empty() == false);
    if (args.size() == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    // note!
    //   gflags will have removed all flags and we're left with arguments
    //   arguments should be a list of unix domain sockets
    auto connections = args.subspan(1);
    // this strategy factory uses direct connectivity to one or more
    // market access gateways
    client::Trader(
        config,
        connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }

  int main(int argc, char **argv) override {
    // wrap arguments (prefer to not work with raw pointers)
    std::vector<std::string_view> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
      args.emplace_back(argv[i]);
    return main_helper(args);
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
      DESCRIPTION,
      ROQ_VERSION).run();
}
