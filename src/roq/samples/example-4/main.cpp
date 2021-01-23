/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-4/controller.h"

namespace {
static const auto DESCRIPTION = "Example 4 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_4::Controller(
             argc, argv, DESCRIPTION, ROQ_VERSION)
      .run();
}
