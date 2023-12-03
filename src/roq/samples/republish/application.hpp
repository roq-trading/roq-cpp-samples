/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace republish {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace republish
}  // namespace samples
}  // namespace roq
