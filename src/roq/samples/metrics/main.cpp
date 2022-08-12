/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/samples/metrics/application.hpp"

using namespace std::literals;

namespace {
auto const DESCRIPTION = "Metrics (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::metrics::Application(
             argc,
             argv,
             {
                 .description = DESCRIPTION,
                 .package_name = ROQ_PACKAGE_NAME,
                 .build_version = ROQ_VERSION,
             })
      .run();
}
