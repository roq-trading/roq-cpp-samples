/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_5 {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace example_5
}  // namespace samples
}  // namespace roq
