/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/client.h"

namespace roq {
namespace samples {
namespace example_7 {

class Collector final : public client::Collector {
 public:
  void operator()(const Event<CustomMetrics> &) override;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq
