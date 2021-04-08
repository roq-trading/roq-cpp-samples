/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-5/producer.h"

#include <cassert>
#include <chrono>
#include <string>

#include "roq/logging.h"

using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace example_5 {

Producer::Producer(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Producer::operator()(const Event<Start> &) {
  assert(!static_cast<bool>(thread_) && !terminating_);
  thread_ = std::make_unique<std::thread>([this]() { run(); });
}

void Producer::operator()(const Event<Stop> &) {
  assert(static_cast<bool>(thread_) && !terminating_);
  terminating_ = true;
  thread_->join();
}

void Producer::run() {
  log::info("producer was started"_sv);
  while (!terminating_) {
    std::string message = "hello world"_s;
    client::CustomMessage custom_message{
        .message = message.data(),
        .length = message.length() + 1,  // including trailining null
    };
    dispatcher_.enqueue(custom_message);
    std::this_thread::sleep_for(100ms);
  }
  log::info("producer was terminated"_sv);
}

}  // namespace example_5
}  // namespace samples
}  // namespace roq
