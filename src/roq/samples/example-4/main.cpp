/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/example-4/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Example 4 (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_4::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
