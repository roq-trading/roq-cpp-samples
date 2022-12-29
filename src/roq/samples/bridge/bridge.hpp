/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace bridge {

class Bridge final : public client::Handler {
 public:
  explicit Bridge(client::Dispatcher &);

  Bridge(Bridge &&) = default;
  Bridge(Bridge const &) = delete;

 protected:
  // these methods have special meaning for a bridge application
  void operator()(Event<Start> const &) override;
  void operator()(Event<Stop> const &) override;
  void operator()(Event<Timer> const &) override;
  // these methods are similar to trader applications
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  // ... etc.

 private:
  [[maybe_unused]] client::Dispatcher &dispatcher_;
};

}  // namespace bridge
}  // namespace samples
}  // namespace roq
