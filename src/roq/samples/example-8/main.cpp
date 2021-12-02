/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-8/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Example 1 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_8::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
