/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/session.hpp"

#include <utility>

#include <nlohmann/json.hpp>

#include "roq/exceptions.hpp"
#include "roq/logging.hpp"

#include "roq/web/socket/server_factory.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

Session::Session(uint64_t session_id, io::net::tcp::Connection::Factory &factory, Shared &shared)
    : session_id_(session_id), server_(web::socket::ServerFactory::create(*this, factory)), shared_(shared) {
}

// web::socket::Server::Handler

void Session::operator()(web::socket::Server::Disconnected const &) {
  shared_.sessions_to_remove.emplace(session_id_);
}

void Session::operator()(web::socket::Server::Ready const &) {
  log::info("Ready!"sv);
}

void Session::operator()(web::socket::Server::Text const &text) {
  log::info(R"(message="{})"sv, text.payload);
  try {
    process_request(text.payload);
    return;  // note!
  } catch (roq::RuntimeError &e) {
    log::error("Error: {}"sv, e);
  } catch (std::exception &e) {
    log::error("Error: {}"sv, e.what());
  }
  (*server_).close();  // note!
}

void Session::operator()(web::socket::Server::Binary const &) {
  log::warn("Unexpected"sv);
  (*server_).close();
}

// utilities

void Session::process_request(std::string_view const &message) {
  auto json = nlohmann::json::parse(message);
  auto action = json.value("action"s, ""s);
  auto symbol = json.value("symbol"s, ""s);
  if (std::empty(action)) {
    send_error("missing 'action'"sv);
  } else if (action == "subscribe"sv) {
    if (validate_symbol(symbol)) {
      // XXX maybe check if symbol already exists?
      shared_.symbols.emplace(symbol);
      send_success();
    }
  } else if (action == "unsubscribe"sv) {
    if (validate_symbol(symbol)) {
      // XXX maybe check if symbol exists?
      shared_.symbols.erase(symbol);
      send_success();
    }
  } else {
    send_error("unknown 'action'"sv);
  }
}

bool Session::validate_symbol(std::string_view const &symbol) {
  if (std::empty(symbol)) {
    send_error("expected 'symbol'"sv);
    return false;
  } else if (std::size(symbol) > sizeof(decltype(Shared::symbols)::value_type)) {
    send_error("symbol length exceeds maximum"sv);
    return false;
  } else {
    return true;
  }
}

void Session::send_success() {
  send(R"({{)"
       R"("status":"successs")"
       R"(}})"sv);
}

void Session::send_error(std::string_view const &text) {
  send(
      R"({{)"
      R"("status":"error",)"
      R"("text":"{}")"
      R"(}})"sv,
      text);
}

template <typename... Args>
void Session::send(fmt::format_string<Args...> const &fmt, Args &&...args) {
  buffer_.clear();
  fmt::format_to(std::back_inserter(buffer_), fmt, std::forward<Args>(args)...);
  std::string_view message{std::data(buffer_), std::size(buffer_)};
  log::info<3>("{}"sv, message);
  (*server_).send_text(message);
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
