/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace rest {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace rest
}  // namespace samples
}  // namespace roq
