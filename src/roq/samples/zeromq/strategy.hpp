/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <arpa/inet.h>

#include "roq/api.hpp"
#include "roq/client.hpp"

// note! thin wrappers around libzmq
#include "roq/samples/zeromq/zmq/context.hpp"
#include "roq/samples/zeromq/zmq/socket.hpp"

namespace roq {
namespace samples {
namespace zeromq {

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = default;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<TopOfBook> &) override;

 private:
  client::Dispatcher &dispatcher_;
  zmq::Context context_;
  zmq::Socket socket_;
};

}  // namespace zeromq
}  // namespace samples
}  // namespace roq
