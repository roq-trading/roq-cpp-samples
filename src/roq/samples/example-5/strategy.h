/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

#include "roq/samples/example-5/producer.h"

namespace roq {
namespace samples {
namespace example_5 {

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &);

  Strategy(Strategy &&) = delete;
  Strategy(const Strategy &) = delete;

 protected:
  void operator()(const Event<Start> &) override;
  void operator()(const Event<Stop> &) override;
  void operator()(const Event<TopOfBook> &) override;
  void operator()(const Event<client::CustomMessage> &) override;

 private:
  client::Dispatcher &dispatcher_;
  Producer producer_;
};

}  // namespace example_5
}  // namespace samples
}  // namespace roq
