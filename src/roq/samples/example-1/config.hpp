/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/example-1/settings.hpp"

namespace roq {
namespace samples {
namespace example_1 {

// client::Config is an interface allowing a strategy factory
// to query for relevant configuration
// the design uses dispatch to allow the factory to use interfaces
// without imposing any kind of requirement on containers and storage

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  Settings const &settings_;
  void dispatch(Handler &) const override;
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq
