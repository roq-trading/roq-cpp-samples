/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/io-context/session.hpp"

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

void Session::operator()(web::socket::Server::Disconnected const &) {
  shared_.sessions_to_remove.emplace(session_id_);
}

void Session::operator()(web::socket::Server::Ready const &) {
  log::info("Ready!"sv);
}

void Session::operator()(web::socket::Server::Text const &text) {
  log::info(R"(message="{})"sv, text.payload);
  try {
    auto json = nlohmann::json::parse(text.payload);
    auto action = json.value("action"s, ""s);
    auto symbol = json.value("symbol"s, ""s);
    if (std::empty(action)) {
      (*server_).send_text(R"({)"
                           R"("status":"error",)"
                           R"("message":"missing 'action'")"
                           R"(})"sv);
    } else if (action == "subscribe"sv) {
      (*server_).send_text(R"({)"
                           R"("status":"successs")"
                           R"(})"sv);
    } else if (action == "unsubscribe"sv) {
      (*server_).send_text(R"({)"
                           R"("status":"successs")"
                           R"(})"sv);
    } else {
      (*server_).send_text(R"({)"
                           R"("status":"error",)"
                           R"("message":"unknown 'action'")"
                           R"(})"sv);
    }
    return;
  } catch (roq::RuntimeError &e) {
    log::error("Error: {}"sv, e);
  } catch (std::exception &e) {
    log::error("Error: {}"sv, e.what());
  }
  (*server_).close();
}

void Session::operator()(web::socket::Server::Binary const &) {
  log::warn("Unexpected"sv);
  (*server_).close();
}

}  // namespace io_context
}  // namespace samples
}  // namespace roq
