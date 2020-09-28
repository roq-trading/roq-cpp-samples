/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-3/controller.h"

namespace {
constexpr std::string_view DESCRIPTION = "Example 3 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_3::Controller(
      argc,
      argv,
      DESCRIPTION,
      ROQ_VERSION).run();
}
