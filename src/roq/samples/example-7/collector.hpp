/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

namespace roq {
namespace samples {
namespace example_7 {

struct Collector final : public client::Collector {
  void operator()(Event<CustomMetricsUpdate> const &) override;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq
