/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/client/config.h"

namespace roq {
namespace samples {
namespace example_3 {

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config &) = delete;
  Config(Config &&) = default;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
