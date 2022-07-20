/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <memory>
#include <vector>

#include "roq/io/net/tcp/connection.hpp"

#include "roq/web/socket/server.hpp"

#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

class Session final : public web::socket::Server::Handler {
 public:
  Session(uint64_t session_id, io::net::tcp::Connection::Factory &, Shared &shared);

 protected:
  // web::socket::Server::Handler
  void operator()(web::socket::Server::Disconnected const &) override;
  void operator()(web::socket::Server::Ready const &) override;
  void operator()(web::socket::Server::Text const &) override;
  void operator()(web::socket::Server::Binary const &) override;

  // utilities

  void process_request(std::string_view const &message);

  bool validate_symbol(std::string_view const &symbol);

  void send_success();
  void send_error(std::string_view const &text);

  template <typename... Args>
  void send(fmt::format_string<Args...> const &, Args &&...);

 private:
  const uint64_t session_id_;
  const std::unique_ptr<roq::web::socket::Server> server_;
  Shared &shared_;
  std::vector<char> buffer_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
