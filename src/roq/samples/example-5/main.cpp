/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-5/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Example 5 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_5::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
