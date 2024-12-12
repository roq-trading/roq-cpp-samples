/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/io-context/session.hpp"

#include <utility>

#include <nlohmann/json.hpp>

#include "roq/exceptions.hpp"
#include "roq/logging.hpp"

#include "roq/web/rest/server.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace io_context {

// === IMPLEMENTATION ===

Session::Session(uint64_t session_id, io::net::tcp::Connection::Factory &factory, Shared &shared)
    : session_id_{session_id}, server_{web::rest::Server::create(*this, factory)}, shared_{shared} {
}

// web::rest::Server::Handler

void Session::operator()(web::rest::Server::Disconnected const &) {
  shared_.sessions_to_remove.emplace(session_id_);
}

void Session::operator()(web::rest::Server::Request const &request) {
  if (request.headers.connection.has(web::http::Connection::UPGRADE)) {
    log::info("Upgrading session_id={} to websocket..."sv, session_id_);
    (*server_).upgrade(request);
  } else {
    log::info("request={}"sv, request);
    try {
      // XXX expect POST
      auto result = process_request(request.body);
      auto connection = [&]() {
        if (request.headers.connection.has(web::http::Connection::KEEP_ALIVE))
          return web::http::Connection::KEEP_ALIVE;
        return web::http::Connection::CLOSE;
      }();
      web::rest::Server::Response response{
          .status = web::http::Status::OK,  // XXX should depend on result type
          .connection = connection,
          .sec_websocket_accept = {},
          .cache_control = {},
          .content_type = web::http::ContentType::APPLICATION_JSON,
          .body = result,
      };
      (*server_).send(response);
    } catch (roq::RuntimeError &e) {
      log::error("Error: {}"sv, e);
      (*server_).close();
    } catch (std::exception &e) {
      log::error("Error: {}"sv, e.what());
      (*server_).close();
    }
  }
}

void Session::operator()(web::rest::Server::Text const &text) {
  log::info(R"(message="{})"sv, text.payload);
  try {
    auto result = process_request(text.payload);
    (*server_).send_text(result);
  } catch (roq::RuntimeError &e) {
    log::error("Error: {}"sv, e);
    (*server_).close();
  } catch (std::exception &e) {
    log::error("Error: {}"sv, e.what());
    (*server_).close();
  }
}

void Session::operator()(web::rest::Server::Binary const &) {
  log::warn("Unexpected"sv);
  (*server_).close();
}

// utilities

std::string_view Session::process_request(std::string_view const &message) {
  auto json = nlohmann::json::parse(message);
  auto action = json.value("action"s, ""s);
  auto symbol = json.value("symbol"s, ""s);
  if (std::empty(action)) {
    return error("missing 'action'"sv);
  } else if (action == "subscribe"sv) {
    if (validate_symbol(symbol)) {
      // XXX maybe check if symbol already exists?
      shared_.symbols.emplace(symbol);
      return success();
    } else {
      return error("invalid symbol"sv);
    }
  } else if (action == "unsubscribe"sv) {
    if (validate_symbol(symbol)) {
      // XXX maybe check if symbol exists?
      shared_.symbols.erase(symbol);
      return success();
    } else {
      return error("invalid symbol"sv);
    }
  } else {
    return error("unknown 'action'"sv);
  }
}

bool Session::validate_symbol(std::string_view const &symbol) {
  if (std::empty(symbol)) {
    return false;
  } else if (std::size(symbol) > sizeof(decltype(Shared::symbols)::value_type)) {
    return false;
  } else {
    return true;
  }
}

std::string_view Session::success() {
  return format(R"({{)"
                R"("status":"successs")"
                R"(}})"sv);
}

std::string_view Session::error(std::string_view const &text) {
  return format(
      R"({{)"
      R"("status":"error",)"
      R"("text":"{}")"
      R"(}})"sv,
      text);
}

template <typename... Args>
std::string_view Session::format(fmt::format_string<Args...> const &fmt, Args &&...args) {
  buffer_.clear();
  fmt::format_to(std::back_inserter(buffer_), fmt, std::forward<Args>(args)...);
  return std::string_view{std::data(buffer_), std::size(buffer_)};
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
