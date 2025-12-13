/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/example-2/settings.hpp"

namespace roq {
namespace samples {
namespace example_2 {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  Settings const &settings_;
  void dispatch(Handler &) const override;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
