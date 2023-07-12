/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_6 {

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
