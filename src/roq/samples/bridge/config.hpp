/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/client/config.hpp"

namespace roq {
namespace samples {
namespace bridge {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(const Config &) = delete;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace bridge
}  // namespace samples
}  // namespace roq
