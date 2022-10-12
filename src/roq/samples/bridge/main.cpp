/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/samples/bridge/application.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
roq::Service::Info const INFO{
    .description = "Bridge (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  return roq::samples::bridge::Application{argc, argv, INFO}.run();
}
