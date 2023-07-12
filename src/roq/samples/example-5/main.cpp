/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/flags/args.hpp"
#include "roq/logging/flags/settings.hpp"

#include "roq/samples/example-5/application.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "Example 5 (Roq Samples)"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  roq::logging::flags::Settings settings{args};
  return roq::samples::example_5::Application{args, settings, INFO}.run();
}
