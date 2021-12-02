/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/client/config.h"

namespace roq {
namespace samples {
namespace example_6 {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(const Config &) = delete;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
