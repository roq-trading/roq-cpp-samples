/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client/config.hpp"

namespace roq {
namespace samples {
namespace bridge {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;
};

}  // namespace bridge
}  // namespace samples
}  // namespace roq
