/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-5/producer.hpp"

#include <cassert>
#include <chrono>
#include <string>

#include "roq/logging.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_5 {

// === IMPLEMENTATION ===

Producer::Producer(client::Dispatcher &dispatcher) : dispatcher_{dispatcher} {
}

void Producer::operator()(Event<Start> const &) {
  assert(!static_cast<bool>(thread_) && !terminating_);
  thread_ = std::make_unique<std::thread>([this]() { run(); });
}

void Producer::operator()(Event<Stop> const &) {
  assert(static_cast<bool>(thread_) && !terminating_);
  terminating_ = true;
  (*thread_).join();
}

void Producer::run() {
  log::info("producer was started"sv);
  while (!terminating_) {
    auto text = "hello world"s;  // note! std::string
    auto message = std::span{
        reinterpret_cast<std::byte const *>(std::data(text)),
        std::size(text) + 1,  // including trailing null
    };
    client::CustomMessage custom_message{
        .message = message,
    };
    dispatcher_.enqueue(custom_message);
    std::this_thread::sleep_for(100ms);
  }
  log::info("producer was terminated"sv);
}

}  // namespace example_5
}  // namespace samples
}  // namespace roq
