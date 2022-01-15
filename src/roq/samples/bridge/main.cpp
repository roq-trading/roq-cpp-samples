/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/bridge/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Bridge (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::bridge::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
