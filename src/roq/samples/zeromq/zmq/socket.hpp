/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <zmq.h>

#include <memory>
#include <string_view>

#include "roq/samples/zeromq/zmq/context.hpp"

namespace roq {
namespace samples {
namespace zeromq {
namespace zmq {

struct Socket final {
  using value_type = void;

  Socket(Context &, int type);

  operator value_type *() { return handle_.get(); }
  operator value_type const *() const { return handle_.get(); }

  // XXX make type-safe
  void setsockopt(int option_name, void const *option_value, size_t option_len);

  void bind(std::string_view const &endpoint);

  void connect(std::string_view const &endpoint);

  size_t send(void const *buf, size_t len, int flags);

 private:
  std::unique_ptr<value_type, void (*)(value_type *)> handle_;
};

}  // namespace zmq
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
