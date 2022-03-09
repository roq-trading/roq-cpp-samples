/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_7 {

class Collector final : public client::Collector {
 public:
  void operator()(const Event<CustomMetricsUpdate> &) override;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq
