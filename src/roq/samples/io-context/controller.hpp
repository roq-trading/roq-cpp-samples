/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <memory>

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/io/net/tcp/connection.hpp"
#include "roq/io/net/tcp/listener.hpp"

#include "roq/samples/io-context/session.hpp"
#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

class Controller final : public client::Handler, public io::net::tcp::Listener::Handler {
 public:
  explicit Controller(client::Dispatcher &, io::Context &);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

 protected:
  void operator()(Event<Timer> const &) override;
  void operator()(Event<TopOfBook> const &) override;

 protected:
  void operator()(io::net::tcp::Connection::Factory &) override;

 private:
  client::Dispatcher &dispatcher_;
  io::Context &context_;
  std::unique_ptr<roq::io::net::tcp::Listener> listener_;
  Shared shared_;
  uint64_t next_session_id_ = {};
  absl::flat_hash_map<uint64_t, std::unique_ptr<Session> > sessions_;
  std::unique_ptr<roq::io::Sender> sender_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
