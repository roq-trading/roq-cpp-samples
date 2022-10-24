/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace metrics {

// client::Config is an interface allowing a strategy factory
// to query for relevant configuration
// the design uses dispatch to allow the factory to use interfaces
// without imposing any kind of requirement on containers and storage

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;
};

}  // namespace metrics
}  // namespace samples
}  // namespace roq
