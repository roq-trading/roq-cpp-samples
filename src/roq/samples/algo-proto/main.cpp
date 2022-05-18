/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/algo-proto/application.hpp"

#include "roq/api.hpp"

using namespace std::literals;

namespace {
auto const DESCRIPTION = "Roq Samples Algo Prototype"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::algo_proto::Application(
             argc,
             argv,
             {
                 .description = DESCRIPTION,
                 .package_name = ROQ_PACKAGE_NAME,
                 .build_version = ROQ_VERSION,
             })
      .run();
}
