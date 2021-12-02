/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-6/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Example 6 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_6::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
