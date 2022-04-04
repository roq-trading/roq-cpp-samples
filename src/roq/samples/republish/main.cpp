/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/samples/republish/application.hpp"

using namespace std::literals;

namespace {
const auto DESCRIPTION = "Republish (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::republish::Application(
             argc,
             argv,
             {
                 .description = DESCRIPTION,
                 .package_name = ROQ_PACKAGE_NAME,
                 .build_version = ROQ_VERSION,
             })
      .run();
}
