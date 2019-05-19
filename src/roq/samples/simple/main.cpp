/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include "roq/application.h"

#include "roq/samples/common/tokenizer.h"

#include "roq/samples/simple/performance.h"
#include "roq/samples/simple/strategy.h"

// common options
DEFINE_string(exchange, "", "Exchange name");
DEFINE_string(accounts, "", "Accounts. A comma-separated list, "
                            "e.g. \"A1,A2\"");
DEFINE_string(symbols, "", "Symbols. One or more comma-separated lists "
                           "prefixed with exchange, "
                           "e.g. \"E1:S1,S2;E2:S3\")");
DEFINE_uint32(create_orders, 0, "Create orders?");
DEFINE_uint32(order_timeout, 5000, "Order timeout (ms)");

// simulation options
DEFINE_bool(simulation, false, "Simulation");
DEFINE_string(generator_type, "csv", "Generator type");
DEFINE_string(generator_name, "CFFEX", "Generator name");
DEFINE_string(matcher_type, "simple", "Matcher type");
DEFINE_uint32(matcher_buffer_size, (64 + 1) * 4096, "Matcher buffer size");
DEFINE_uint32(market_data_latency, 10, "Market data latency (ms)");
DEFINE_uint32(order_manager_latency, 100, "Order manager latency (ms)");

namespace {
constexpr const char *DESCRIPTION = "Simple Strategy (Roq Samples)";
}  // namespace

namespace roq {
namespace samples {
namespace simple {

class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  int main(int argc, char **argv) override {
    // arguments represent either
    // - simulation files, or
    // - gateway connection parameters
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    std::vector<std::string> connections(argv + 1, argv + argc);
    // parse optios
    // - accounts
    auto accounts = roq::samples::common::tokenize(FLAGS_accounts, ",");
    if (std::size(accounts) != 1)
      throw std::runtime_error("Expected exactly 1 account");
    // - symbols
    auto symbols = roq::samples::common::tokenize(FLAGS_symbols, ",");
    if (std::size(symbols) != 1)
      throw std::runtime_error("Expected exactly 1 symbol");
    // - create-orders flag
    auto create_orders = FLAGS_create_orders != 0;
    // - order timeout
    std::chrono::milliseconds order_timeout { FLAGS_order_timeout };
    // create strategy + start event dispatcher
    create_and_dispatch(
        connections,
        FLAGS_exchange,
        accounts,
        symbols,
        create_orders,
        order_timeout);
    return EXIT_SUCCESS;
  }

  template <typename T, typename... Args>
  void create_and_dispatch(const T& connections, Args&&... args) {
    if (FLAGS_simulation) {
      simulate(connections, std::forward<Args>(args)...);
    } else {
      trade(connections, std::forward<Args>(args)...);
    }
  }

  template <typename T, typename... Args>
  void simulate(const T& connections, Args&&... args) {
    using roq::client::detail::SimulationFactory;
    // parse options
    std::chrono::milliseconds market_data_latency {
      FLAGS_market_data_latency };
    std::chrono::milliseconds order_manager_latency {
        FLAGS_order_manager_latency };
    // create the event generator
    auto generator = SimulationFactory::create_generator(
        FLAGS_generator_type,
        connections);
    // create the matcher
    auto matcher = SimulationFactory::create_matcher(
        FLAGS_matcher_type,
        FLAGS_generator_name,
        market_data_latency,
        order_manager_latency,
        FLAGS_matcher_buffer_size);
    // create the collector
    Performance performance;
    // create the strategy and dispatch
    roq::client::Simulator(*generator, *matcher, performance)
      .dispatch<Strategy>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  void trade(const T& connections, Args&&... args) {
    // create and dispatch
    roq::client::Trader(connections)
      .dispatch<Strategy>(std::forward<Args>(args)...);
  }
};

}  // namespace simple
}  // namespace samples
}  // namespace roq

int main(int argc, char **argv) {
  // use the generic application wrapper to e.g.
  // - parse command-line options
  // - configure the logger
  // - set up termination handler
  return roq::samples::simple::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
