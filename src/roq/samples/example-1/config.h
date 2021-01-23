/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_1 {

// client::Config is an interface allowing a strategy factory
// to query for relevant configuration
// the design uses dispatch to allow the factory to use interfaces
// without imposing any kind of requirement on containers and storage

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config &) = delete;
  Config(Config &&) = default;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq
