/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/logging.h"
#include "roq/service.h"

#include "roq/client.h"

DEFINE_string(exchange, "deribit", "exchange name");

DEFINE_string(symbols, ".*", "regex used to subscribe symbols");

namespace roq {
namespace samples {
namespace example_4 {

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config &) = delete;
  Config(Config &&) = default;

 protected:
  void dispatch(Handler &handler) const override {
    handler(client::Symbol{
        .regex = FLAGS_symbols,
        .exchange = FLAGS_exchange,
    });
  }
};

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {}

  Strategy(const Strategy &) = delete;
  Strategy(Strategy &&) = default;

 protected:
  void operator()(const Event<Connection> &event) override {
    LOG(INFO)
    (R"([{}:{}] Connection={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<DownloadBegin> &event) override {
    LOG(INFO)
    (R"([{}:{}] DownloadBegin={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<DownloadEnd> &event) override {
    LOG(INFO)
    (R"([{}:{}] DownloadEnd={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<MarketDataStatus> &event) override {
    LOG(INFO)
    (R"([{}:{}] MarketDataStatus={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<OrderManagerStatus> &event) override {
    LOG(INFO)
    (R"([{}:{}] OrderManagerStatus={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<ReferenceData> &event) override {
    LOG(INFO)
    (R"([{}:{}] ReferenceData={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<MarketStatus> &event) override {
    LOG(INFO)
    (R"([{}:{}] MarketStatus={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<MarketByPriceUpdate> &event) override {
    VLOG(1)
    (R"([{}:{}] MarketByPriceUpdate={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<MarketByOrderUpdate> &event) override {
    VLOG(1)
    (R"([{}:{}] MarketByOrderUpdate={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }
  void operator()(const Event<TradeSummary> &event) override {
    VLOG(1)
    (R"([{}:{}] TradeSummary={})",
     event.message_info.source,
     event.message_info.source_name,
     event.value);
  }

 private:
  client::Dispatcher &dispatcher_;
};

class Controller final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view> &args) {
    assert(args.empty() == false);
    if (args.size() == 1) throw std::runtime_error("Expected arguments");
    Config config;
    auto connections = args.subspan(1);
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }

  int main(int argc, char **argv) override {
    std::vector<std::string_view> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) args.emplace_back(argv[i]);
    return main_helper(args);
  }
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq

namespace {
constexpr std::string_view DESCRIPTION = "Example 4 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_4::Controller(
             argc, argv, DESCRIPTION, ROQ_VERSION)
      .run();
}
