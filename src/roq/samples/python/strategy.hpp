/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <pybind11/embed.h>

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace python {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = delete;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Timer> const &) override;
  void operator()(Event<TopOfBook> const &) override;

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  pybind11::scoped_interpreter interpreter_;
  pybind11::module module_;
  std::chrono::nanoseconds next_timer_ = {};
  Layer layer_;
};

}  // namespace python
}  // namespace samples
}  // namespace roq
