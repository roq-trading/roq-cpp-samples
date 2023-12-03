/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace metrics {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace metrics
}  // namespace samples
}  // namespace roq
