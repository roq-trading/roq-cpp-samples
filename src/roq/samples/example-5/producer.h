/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_5 {

class Producer final {
 public:
  explicit Producer(client::Dispatcher &);

  Producer(Producer &&) = delete;
  Producer(const Producer &) = delete;

  void operator()(const Event<Start> &);
  void operator()(const Event<Stop> &);

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
