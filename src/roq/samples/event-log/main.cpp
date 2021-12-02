/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/api.h"

#include "roq/samples/event-log/application.h"

using namespace std::literals;

namespace {
static const auto DESCRIPTION = "Event-Log (Roq Samples)"sv;
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::event_log::Application(argc, argv, DESCRIPTION, ROQ_VERSION).run();
}
