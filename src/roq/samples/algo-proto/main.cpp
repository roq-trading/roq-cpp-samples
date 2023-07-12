/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/algo-proto/application.hpp"

#include "roq/flags/args.hpp"
#include "roq/logging/flags/settings.hpp"

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
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  roq::logging::flags::Settings settings{args};
  return roq::samples::algo_proto::Application{args, settings, INFO}.run();
}
