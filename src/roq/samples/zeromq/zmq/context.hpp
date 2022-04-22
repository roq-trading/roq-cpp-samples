/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <zmq.h>

#include <memory>

namespace roq {
namespace samples {
namespace zeromq {
namespace zmq {

struct Context final {
  using value_type = void;

  Context();

  operator value_type *() { return handle_.get(); }
  operator value_type const *() const { return handle_.get(); }

  void set(int option_name, int option_value);

  int get(int option_name);

 private:
  std::unique_ptr<value_type, void (*)(value_type *)> handle_;
};

}  // namespace zmq
}  // namespace zeromq
}  // namespace samples
}  // namespace roq
