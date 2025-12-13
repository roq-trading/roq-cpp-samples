/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <memory>
#include <vector>

#include "roq/io/net/tcp/connection.hpp"

#include "roq/web/rest/server.hpp"

#include "roq/samples/io-context/shared.hpp"

namespace roq {
namespace samples {
namespace io_context {

// note! supports both rest and websocket

struct Session final : public web::rest::Server::Handler {
  Session(uint64_t session_id, io::net::tcp::Connection::Factory &, Shared &);

 protected:
  // web::rest::Server::Handler
  void operator()(web::rest::Server::Disconnected const &) override;
  void operator()(web::rest::Server::Request const &) override;
  void operator()(web::rest::Server::Text const &) override;
  void operator()(web::rest::Server::Binary const &) override;

  // utilities

  std::string_view process_request(std::string_view const &message);

  bool validate_symbol(std::string_view const &symbol);

  std::string_view success();
  std::string_view error(std::string_view const &text);

  template <typename... Args>
  std::string_view format(fmt::format_string<Args...> const &, Args &&...);

 private:
  uint64_t const session_id_;
  std::unique_ptr<roq::web::rest::Server> server_;
  Shared &shared_;
  std::vector<char> buffer_;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
