/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/example-5/producer.hpp"

namespace roq {
namespace samples {
namespace example_5 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = delete;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Start> const &) override;
  void operator()(Event<Stop> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<client::CustomMessage> const &) override;

 private:
  client::Dispatcher &dispatcher_;
  Producer producer_;
};

}  // namespace example_5
}  // namespace samples
}  // namespace roq
