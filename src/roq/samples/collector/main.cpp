/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/application.h"

#include "roq/samples/collector/collector.h"

namespace {
constexpr const char *DESCRIPTION = "Collector (Roq Samples)";
}  // namespace

namespace roq {
namespace samples {
namespace collector {

class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  class Config final : public client::Config {
   protected:
    void dispatch(Handler&) const override {
      // HANS -- TODO(thraneh): subscribe all symbols
    }
  };
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> arguments(argv + 1, argv + argc);
    roq::client::Trader(config, arguments).dispatch<Collector>();
    return EXIT_SUCCESS;
  }
};

}  // namespace collector
}  // namespace samples
}  // namespace roq

int main(int argc, char **argv) {
  return roq::samples::collector::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
