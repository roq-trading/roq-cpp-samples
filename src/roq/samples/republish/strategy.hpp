/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <arpa/inet.h>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace republish {

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  ~Strategy();

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<TopOfBook> const &) override;

 private:
  client::Dispatcher &dispatcher_;
  struct sockaddr_in address_ = {};
  int socket_ = {};
};

}  // namespace republish
}  // namespace samples
}  // namespace roq
