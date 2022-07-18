/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <memory>
#include <vector>

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/io/net/tcp/connection.hpp"
#include "roq/io/net/tcp/listener.hpp"

#include "roq/io/net/udp/sender.hpp"

#include "roq/samples/io-context/session.hpp"
#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

class Controller final : public client::Handler,
                         public io::net::udp::Sender::Handler,
                         public io::net::tcp::Listener::Handler {
 public:
  explicit Controller(client::Dispatcher &, io::Context &);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

 protected:
  void operator()(Event<Timer> const &) override;
  void operator()(Event<TopOfBook> const &) override;

  template <typename... Args>
  void send(fmt::format_string<Args...> const &, Args &&...);

 protected:
  void operator()(io::net::udp::Sender::Error const &) override;

 protected:
  void operator()(io::net::tcp::Connection::Factory &) override;

 private:
  client::Dispatcher &dispatcher_;
  io::Context &context_;
  std::unique_ptr<io::net::tcp::Listener> listener_;
  Shared shared_;
  std::chrono::nanoseconds next_garbage_collection_ = {};
  uint64_t next_session_id_ = {};
  absl::flat_hash_map<uint64_t, std::unique_ptr<Session> > sessions_;
  std::unique_ptr<io::Sender> sender_;
  std::vector<char> buffer_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
