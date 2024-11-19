/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/rest/controller.hpp"

#include <utility>

#include "roq/logging.hpp"

#include "roq/utils/json/context.hpp"
#include "roq/utils/json/top_of_book.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace rest {

// === CONSTANTS ===

namespace {
auto const REQUEST_TIMEOUT = 5s;
auto const USER_AGENT = "roq_samples"sv;
auto const PING_FREQ = 10s;
auto const DECODE_BUFFER_SIZE = 1048576uz;
auto const ENCODE_BUFFER_SIZE = 1048576uz;
auto const PATH = "/api/v3/avgPrice"sv;
auto const REQUEST_FREQ = 5s;
}  // namespace

// === HELPERS ===

namespace {
auto create_client(auto &handler, auto &settings, auto &context) {
  io::web::URI uri{settings.rest_uri};
  auto config = web::rest::Client::Config{
      // connection
      .interface = {},
      .proxy = {},
      .uris = {&uri, 1},
      .host = {},
      .validate_certificate = {},
      // connection manager
      .connection_timeout = {},
      .disconnect_on_idle_timeout = {},
      .connection = web::http::Connection::KEEP_ALIVE,
      // request
      .allow_pipelining = true,
      .request_timeout = REQUEST_TIMEOUT,
      // response
      .suspend_on_retry_after = true,
      // http
      .query = {},
      .user_agent = USER_AGENT,
      .ping_frequency = PING_FREQ,
      .ping_path = settings.rest_ping_path,
      // implementation
      .decode_buffer_size = DECODE_BUFFER_SIZE,
      .encode_buffer_size = ENCODE_BUFFER_SIZE,
  };
  return web::rest::Client::create(handler, context, config);
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(client::Dispatcher &dispatcher, Settings const &settings, io::Context &context)
    : dispatcher_{dispatcher}, settings_{settings}, context_{context}, client_{create_client(*this, settings, context)} {
}

// client::Handler

void Controller::operator()(Event<Start> const &) {
  (*client_).start();
}

void Controller::operator()(Event<Stop> const &) {
  (*client_).stop();
}

void Controller::operator()(Event<Timer> const &event) {
  auto now = event.value.now;
  context_.drain();
  (*client_).refresh(now);
  if (next_request_time_ < now) {
    next_request_time_ = now + REQUEST_FREQ;
    try_request();
  }
}

void Controller::operator()(Event<TopOfBook> const &) {
}

// web::rest::Client::Handler

void Controller::operator()(Trace<web::rest::Client::Connected> const &) {
  (*this)(State::CONNECTED);
}

void Controller::operator()(Trace<web::rest::Client::Disconnected> const &) {
  (*this)(State::DISCONNECTED);
}

void Controller::operator()(Trace<web::rest::Client::Latency> const &event) {
  log::info("latency={}"sv, event.value.sample);
}

// helpers

void Controller::operator()(State state) {
  state_ = state;
  log::info("state={}"sv, magic_enum::enum_name(state_));
}

void Controller::try_request() {
  auto path = fmt::format("{}?symbol={}"sv, PATH, settings_.symbol);
  auto request = web::rest::Request{
      .method = web::http::Method::GET,
      .path = path,
      .query = {},
      .accept = web::http::Accept::APPLICATION_JSON,
      .content_type = {},
      .headers = {},
      .body = {},
      .quality_of_service = {},
  };
  log::info("request={}"sv, request);
  auto callback = [this]([[maybe_unused]] auto &request_id, auto &response) {
    log::debug("response={}"sv, response);
    (*this)(response);
  };
  (*client_)("test"sv, request, callback);
}

void Controller::operator()(web::rest::Response const &response) {
  try {
    auto [status, category, body] = response.result();
    log::info(R"(status={}, category={}, body="{}")"sv, status, category, body);
    switch (category) {
      using enum web::http::Category;
      case SUCCESS:  // 2xx
        break;
      case CLIENT_ERROR:  // 4xx
        break;
      case SERVER_ERROR:  // 5xx
        break;
      default:
        break;
    }
  } catch (NetworkError &e) {
    log::warn(R"(Exception type={}, what="{}")"sv, typeid(e).name(), e.what());
  } catch (std::exception &e) {
    log::warn(R"(Exception type={}, what="{}")"sv, typeid(e).name(), e.what());
  }
}

}  // namespace rest
}  // namespace samples
}  // namespace roq
