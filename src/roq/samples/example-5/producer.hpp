/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_5 {

class Producer final {
 public:
  explicit Producer(client::Dispatcher &);

  Producer(Producer &&) = delete;
  Producer(Producer const &) = delete;

  void operator()(Event<Start> const &);
  void operator()(Event<Stop> const &);

 protected:
  void run();

 private:
  client::Dispatcher &dispatcher_;
  std::unique_ptr<std::thread> thread_;
  std::atomic<bool> terminating_ = {false};
};

}  // namespace example_5
}  // namespace samples
}  // namespace roq
