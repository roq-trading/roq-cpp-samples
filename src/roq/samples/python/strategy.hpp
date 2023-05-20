/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/python/producer.hpp"

namespace roq {
namespace samples {
namespace python {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = delete;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Start> const &) override;
  void operator()(Event<Stop> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<client::CustomMessage> const &) override;

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
  Producer producer_;
};

}  // namespace python
}  // namespace samples
}  // namespace roq
