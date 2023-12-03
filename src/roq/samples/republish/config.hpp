/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/samples/republish/settings.hpp"

namespace roq {
namespace samples {
namespace republish {

struct Config final : public client::Config {
  explicit Config(Settings const &);

  Config(Config &&) = default;
  Config(Config const &) = delete;

 protected:
  void dispatch(Handler &) const override;

 private:
  Settings const &settings_;
};

}  // namespace republish
}  // namespace samples
}  // namespace roq
