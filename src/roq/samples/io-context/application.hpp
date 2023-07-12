/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace io_context {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
