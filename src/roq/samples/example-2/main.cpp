/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/samples/example-2/application.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Example 2 (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  return roq::samples::example_2::Application{argc, argv, INFO}.run();
}
