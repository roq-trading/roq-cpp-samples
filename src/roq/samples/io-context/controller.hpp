/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <chrono>
#include <memory>
#include <vector>

#include "roq/client.hpp"

#include "roq/utils/container.hpp"

#include "roq/io/context.hpp"

#include "roq/io/net/tcp/connection.hpp"
#include "roq/io/net/tcp/listener.hpp"

#include "roq/io/net/udp/sender.hpp"

#include "roq/samples/io-context/session.hpp"
#include "roq/samples/io-context/settings.hpp"
#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

struct Controller final : public client::Handler, public io::net::udp::Sender::Handler, public io::net::tcp::Listener::Handler {
  Controller(client::Dispatcher &, Settings const &, io::Context &);

 protected:
  // client::Handler
  void operator()(Event<Timer> const &) override;
  void operator()(Event<TopOfBook> const &) override;

  // io::net::udp::Sender::Handler
  void operator()(io::net::udp::Sender::Error const &) override;

  // io::net::tcp::Listener::Handler
  void operator()(io::net::tcp::Connection::Factory &) override;
  void operator()(io::net::tcp::Connection::Factory &, io::NetworkAddress const &) override;

  // utilities

  void remove_zombies(std::chrono::nanoseconds now);

  template <typename... Args>
  void send(fmt::format_string<Args...> const &, Args &&...);

 private:
  client::Dispatcher &dispatcher_;
  Settings const &settings_;
  io::Context &context_;
  std::unique_ptr<io::Sender> const sender_;
  std::unique_ptr<io::net::tcp::Listener> const listener_;
  utils::unordered_map<uint64_t, std::unique_ptr<Session>> sessions_;
  std::chrono::nanoseconds next_garbage_collection_ = {};
  uint64_t next_session_id_ = {};
  Shared shared_;
  std::vector<char> buffer_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
