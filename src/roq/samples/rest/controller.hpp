/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/web/rest/client.hpp"

#include "roq/samples/rest/session.hpp"
#include "roq/samples/rest/settings.hpp"

namespace roq {
namespace samples {
namespace rest {

struct Controller final : public client::Handler, public web::rest::Client::Handler {
  Controller(client::Dispatcher &, Settings const &, io::Context &);

 protected:
  // client::Handler
  void operator()(Event<Start> const &) override;
  void operator()(Event<Stop> const &) override;
  void operator()(Event<Timer> const &) override;
  void operator()(Event<TopOfBook> const &) override;

  // web::rest::Client::Handler
  void operator()(Trace<web::rest::Client::Connected> const &) override;
  void operator()(Trace<web::rest::Client::Disconnected> const &) override;
  void operator()(Trace<web::rest::Client::Latency> const &) override;

  // helpers

  enum class State {
    CONNECTED,
    DISCONNECTED,
  };

  void operator()(State);

  void try_request();

  void operator()(web::rest::Response const &);

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  Settings const &settings_;
  io::Context &context_;
  std::unique_ptr<web::rest::Client> client_;
  State state_ = {};
  std::chrono::nanoseconds next_request_time_ = {};
};

}  // namespace rest
}  // namespace samples
}  // namespace roq
