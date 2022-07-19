/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/session.hpp"

#include <nlohmann/json.hpp>

#include "roq/logging.hpp"

#include "roq/web/socket/server_factory.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

Session::Session(uint64_t session_id, io::net::tcp::Connection::Factory &factory, Shared &shared)
    : session_id_(session_id), server_(web::socket::ServerFactory::create(*this, factory)), shared_(shared) {
}

void Session::operator()(web::socket::Server::Disconnected const &) {
  shared_.sessions_to_remove.emplace(session_id_);
}

void Session::operator()(web::socket::Server::Ready const &) {
  log::info("Ready!"sv);
}

void Session::operator()(web::socket::Server::Text const &text) {
  log::info(R"(message="{})"sv, text.payload);
  auto json = nlohmann::json::parse(text.payload);
  auto greeting = json.value("hello"s, ""s);
  (*server_).send_text(R"({"hello":"world!"})"sv);
}

void Session::operator()(web::socket::Server::Binary const &) {
  log::warn("Unexpected"sv);
  (*server_).close();
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
