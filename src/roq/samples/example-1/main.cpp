/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-1/application.h"

using namespace std::literals;  // NOLINT

namespace {
static const auto DESCRIPTION = "Example 1 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_1::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
