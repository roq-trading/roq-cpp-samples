/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_7 {

class Collector final : public client::Collector {
 public:
  void operator()(Event<CustomMetricsUpdate> const &) override;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq
