/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/flags/args.hpp"

#include "roq/logging.hpp"

#include "roq/logging/flags/settings.hpp"

#include "roq/service.hpp"

#include "roq/client.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Minimal (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};

auto const ORDER_CANCEL_POLICY = roq::OrderCancelPolicy::BY_ACCOUNT;

auto const ACCOUNT = "A1"sv;

auto const EXCHANGE = "deribit"sv;
auto const SYMBOL = "BTC-PERPETUAL"sv;
}  // namespace

// === HELPERS ===

namespace {

// A Config object is used to define static subscriptions and various policies.

struct Config final : public roq::client::Config {
 protected:
  // When requested, this method must call back with static subscriptions and various policies.
  void dispatch(Handler &handler) const override {
    // Define settings.
    handler(
        roq::client::Settings{
            .order_cancel_policy = ORDER_CANCEL_POLICY,
            .order_management = {},
        });
    // Define accounts.
    handler(
        roq::client::Account{
            .regex = ACCOUNT,
        });
    // Define static subscriptions.
    handler(
        roq::client::Symbol{
            .regex = SYMBOL,
            .exchange = EXCHANGE,
        });
  }
};

// A Strategy object is used to implement event handlers.

struct Strategy final : public roq::client::Handler {
  // The dispatcher is used for sending requests, e.g. order actions.
  explicit Strategy(roq::client::Dispatcher &dispatcher) : dispatcher_{dispatcher} {}

 protected:
  // These are the event handlers we choose to implement (note! there are more):

  void operator()(roq::Event<roq::Connected> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::Disconnected> const &event) override {
    print<0>(event);
    ready_ = false;
  }

  void operator()(roq::Event<roq::DownloadBegin> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::DownloadEnd> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::Ready> const &event) override {
    print<0>(event);
    ready_ = true;
    cancel_all_orders();
  }

  // market data

  void operator()(roq::Event<roq::ReferenceData> const &event) override { print<1>(event); }

  void operator()(roq::Event<roq::MarketStatus> const &event) override { print<1>(event); }

  void operator()(roq::Event<roq::TopOfBook> const &event) override {
    print<1>(event);
    if (ready_) {
      // TODO: It is possible to send order actions only when "ready".
    }
  }

  void operator()(roq::Event<roq::MarketByPriceUpdate> const &event) override { print<1>(event); }

  void operator()(roq::Event<roq::MarketByOrderUpdate> const &event) override { print<1>(event); }

  void operator()(roq::Event<roq::TradeSummary> const &event) override { print<1>(event); }

  void operator()(roq::Event<roq::StatisticsUpdate> const &event) override { print<1>(event); }

  // order management

  void operator()(roq::Event<roq::CancelAllOrdersAck> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::OrderAck> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::OrderUpdate> const &event) override { print<0>(event); }

  void operator()(roq::Event<roq::TradeUpdate> const &event) override { print<0>(event); }

  // Debug logging.
  // Note that the ROQ_v environment variable controls what levels are being logged.
  // With ROQ_v=1 you will see all logging with level<=1, for example.

  template <size_t level, typename T>
  void print(roq::Event<T> const &event) {
    roq::log::info<level>("event={}"sv, event);
  }

  void cancel_all_orders() {
    auto cancel_all_orders = roq::CancelAllOrders{
        .account = ACCOUNT,
        .order_id = {},
        .exchange = EXCHANGE,
        .symbol = SYMBOL,
        .strategy_id = {},
        .side = {},
    };
    dispatcher_.send(cancel_all_orders, 0);
  }

 private:
  roq::client::Dispatcher &dispatcher_;
  bool ready_ = false;
};

// An Application object is used to configure an appropriate environment, including a logging facility.

struct Application final : public roq::Service {
  using roq::Service::Service;

 protected:
  int main(roq::args::Parser const &args) override {
    // Params are command-line arguments which hasn't been parsed as flags (options).
    auto params = args.params();
    // We require at least one connection parameter (normally the path to a UNIX socket).
    if (std::empty(params)) {
      roq::log::fatal("Expected arguments"sv);
    }
    // Parse Roq's client flags.
    roq::client::flags::Settings settings{args};
    // We need a Config object (see above).
    Config config;
    // Create a Strategy and run the dispatch loop.
    roq::client::Trader{settings, config, params}.dispatch<Strategy>();

    return EXIT_SUCCESS;
  }
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  // Parse the command-line.
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  // Initialize the logger.
  roq::logging::flags::Settings settings{args};
  // Create an Application and run the dispatch loop.
  return Application{args, settings, INFO}.run();
}
