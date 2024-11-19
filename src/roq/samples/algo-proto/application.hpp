/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

struct Application final : public roq::Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
