/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/algo-proto/application.hpp"

#include "roq/api.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Roq Samples Algo Prototype"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  return roq::samples::algo_proto::Application{argc, argv, INFO}.run();
}
