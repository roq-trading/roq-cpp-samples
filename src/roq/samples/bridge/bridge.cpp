/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/bridge/bridge.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace bridge {

Bridge::Bridge(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Bridge::operator()(const Event<Start> &) {
  // here you can create (initialize, start threads, etc.)
}

void Bridge::operator()(const Event<Stop> &) {
  // here you can tear down (destroy, stop threads, etc.)
}

void Bridge::operator()(const Event<Timer> &) {
  // this is where you can poll other resources
}

void Bridge::operator()(const Event<Connected> &) {
  // connected to gateway
}

void Bridge::operator()(const Event<Disconnected> &) {
  // disconnected from gateway
}

}  // namespace bridge
}  // namespace samples
}  // namespace roq
