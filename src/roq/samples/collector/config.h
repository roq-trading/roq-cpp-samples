/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/client.h"

namespace roq {
namespace samples {
namespace collector {

class Config final : public client::Config {
 protected:
  void dispatch(Handler& handler) const override;
};

}  // namespace collector
}  // namespace samples
}  // namespace roq
