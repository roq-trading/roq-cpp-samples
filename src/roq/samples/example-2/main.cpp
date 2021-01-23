/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <roq/api.h>

#include "roq/samples/example-2/application.h"

namespace {
static const auto DESCRIPTION = "Example 2 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_2::Application(
             argc, argv, DESCRIPTION, ROQ_VERSION)
      .run();
}
