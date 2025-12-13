/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <arpa/inet.h>

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/republish/settings.hpp"

namespace roq {
namespace samples {
namespace republish {

// strategy implementation

struct Strategy final : public client::Handler {
  Strategy(client::Dispatcher &, Settings const &);

  ~Strategy();

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<TopOfBook> const &) override;

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  struct sockaddr_in address_ = {};
  int socket_ = {};
};

}  // namespace republish
}  // namespace samples
}  // namespace roq
