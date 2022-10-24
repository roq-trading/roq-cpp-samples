/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/bridge/bridge.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace bridge {

// === IMPLEMENTATION ===

Bridge::Bridge(client::Dispatcher &dispatcher) : dispatcher_{dispatcher} {
}

void Bridge::operator()(Event<Start> const &) {
  // here you can create (initialize, start threads, etc.)
}

void Bridge::operator()(Event<Stop> const &) {
  // here you can tear down (destroy, stop threads, etc.)
}

void Bridge::operator()(Event<Timer> const &) {
  // this is where you can poll other resources
}

void Bridge::operator()(Event<Connected> const &) {
  // connected to gateway
}

void Bridge::operator()(Event<Disconnected> const &) {
  // disconnected from gateway
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
