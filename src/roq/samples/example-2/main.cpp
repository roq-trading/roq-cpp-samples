/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <roq/api.h>

#include "roq/samples/example-2/application.h"

using namespace std::literals;

namespace {
const auto DESCRIPTION = "Example 2 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_2::Application(
             argc,
             argv,
             {
                 .description = DESCRIPTION,
                 .package_name = ROQ_PACKAGE_NAME,
                 .build_version = ROQ_VERSION,
             })
      .run();
}
