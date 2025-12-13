/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/polling/settings.hpp"

namespace roq {
namespace samples {
namespace polling {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  Settings const &settings_;
  void dispatch(Handler &) const override;
};

}  // namespace polling
}  // namespace samples
}  // namespace roq
