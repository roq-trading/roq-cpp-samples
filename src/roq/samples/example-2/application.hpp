/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq
