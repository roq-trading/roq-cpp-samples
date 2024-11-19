/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/vwap/application.hpp"

#include "roq/samples/vwap/processor.hpp"
#include "roq/samples/vwap/settings.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace vwap {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::size(params) < 1)
    log::fatal("Expected at least 1 argument"sv);
  Settings settings{args};
  for (auto &path : params)
    Processor::dispatch(settings, path);
  return EXIT_SUCCESS;
}

}  // namespace vwap
}  // namespace samples
}  // namespace roq
