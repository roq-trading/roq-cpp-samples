/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/python/producer.hpp"

#include <cassert>
#include <chrono>
#include <string>

#include "roq/logging.hpp"

using namespace std::literals;
using namespace std::chrono_literals;

namespace roq {
namespace samples {
namespace python {

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
  pybind11::scoped_interpreter guard;
  pybind11::print("Hello, World!");
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

}  // namespace python
}  // namespace samples
}  // namespace roq
