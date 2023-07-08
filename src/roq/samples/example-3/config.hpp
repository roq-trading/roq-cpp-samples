/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client/config.hpp"

#include "roq/samples/example-3/settings.hpp"

namespace roq {
namespace samples {
namespace example_3 {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  Settings const &settings_;
  void dispatch(Handler &) const override;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
