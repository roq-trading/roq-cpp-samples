/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/io/net/tcp/connection.hpp"

#include "roq/web/socket/server.hpp"

#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

class Session final : public web::socket::Server::Handler {
 public:
  Session(uint64_t session_id, io::net::tcp::Connection::Factory &, Shared &shared);

  Session(Session &&) = default;
  Session(Session const &) = delete;

 protected:
  // web::socket::Server::Handler
  void operator()(web::socket::Server::Disconnected const &) override;
  void operator()(web::socket::Server::Ready const &) override;
  void operator()(web::socket::Server::Text const &) override;
  void operator()(web::socket::Server::Binary const &) override;

 private:
  const uint64_t session_id_;
  const std::unique_ptr<roq::web::socket::Server> server_;
  Shared &shared_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
