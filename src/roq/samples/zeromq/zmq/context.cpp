/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/zeromq/zmq/context.hpp"

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
    while (true) {
      if (zmq_ctx_term(ptr) != EINTR)
        return;
    }
  }
}

template <typename R>
R create() {
  R result{zmq_ctx_new(), deleter};
  if (!result)
    throw SystemError{std::make_error_code(static_cast<std::errc>(errno)), "zmq_ctx_new"sv};
  return result;
}
}  // namespace

Context::Context() : handle_(create<decltype(handle_)>()) {
}

void Context::set(int option_name, int option_value) {
  auto result = zmq_ctx_set(handle_.get(), option_name, option_value);
  if (result == -1)
    throw RuntimeError{"zmq_ctx_set"sv};
}

int Context::get(int option_name) {
  auto result = zmq_ctx_get(handle_.get(), option_name);
  if (result == -1)
    throw RuntimeError{"zmq_ctx_get"sv};
  return result;
}

}  // namespace zmq
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
