/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/zeromq/zmq/socket.hpp"

#include <cassert>
#include <string>

#include "roq/exceptions.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace zeromq {
namespace zmq {

namespace {
using value_type = Context::value_type;

void deleter(value_type *ptr) {
  if (ptr) {
    zmq_close(ptr);
  }
}

template <typename R>
R create(Context &context, int type) {
  R result{zmq_socket(context, type), deleter};
  if (!result)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_socket"sv};
  return result;
}
}  // namespace

Socket::Socket(Context &context, int type) : handle_(create<decltype(handle_)>(context, type)) {
}

void Socket::setsockopt(int option_name, void const *option_value, size_t option_len) {
  auto result = zmq_setsockopt(handle_.get(), option_name, option_value, option_len);
  if (result == -1)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_setsockopt"sv};
  assert(result == 0);
}

void Socket::bind(std::string_view const &endpoint) {
  std::string tmp{endpoint};
  auto result = zmq_bind(handle_.get(), tmp.c_str());
  if (result == -1)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_bind"sv};
  assert(result == 0);
}

void Socket::connect(std::string_view const &endpoint) {
  std::string tmp{endpoint};
  auto result = zmq_connect(handle_.get(), tmp.c_str());
  if (result == -1)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_connect"sv};
  assert(result == 0);
}

size_t Socket::send(void const *buf, size_t len, int flags) {
  auto result = zmq_send(handle_.get(), buf, len, flags);
  if (result == -1)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_send"sv};
  assert(result >= 0);
  return result;
}

}  // namespace zmq
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
