/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/samples/io-context/application.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
roq::Service::Info const INFO{
    .description = "I/O Context (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  return roq::samples::io_context::Application{argc, argv, INFO}.run();
}
