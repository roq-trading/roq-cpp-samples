/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_2 {

class Config final : public client::Config {
 public:
  Config() {}

  Config(Config &&) = default;
  Config(const Config &) = delete;

 protected:
  void dispatch(Handler &handler) const override;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
