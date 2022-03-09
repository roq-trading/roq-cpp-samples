/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace bridge {

class Bridge final : public client::Handler {
 public:
  explicit Bridge(client::Dispatcher &);

  Bridge(Bridge &&) = default;
  Bridge(const Bridge &) = delete;

 protected:
  // these methods have special meaning for a bridge application
  void operator()(const Event<Start> &) override;
  void operator()(const Event<Stop> &) override;
  void operator()(const Event<Timer> &) override;
  // these methods are similar to trader applications
  void operator()(const Event<Connected> &) override;
  void operator()(const Event<Disconnected> &) override;
  // ... etc.

 private:
  client::Dispatcher &dispatcher_;
};

}  // namespace bridge
}  // namespace samples
}  // namespace roq
