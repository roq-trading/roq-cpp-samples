/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client/config.hpp"

namespace roq {
namespace samples {
namespace example_6 {

struct Config final : public client::Config {
  Config() {}

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
