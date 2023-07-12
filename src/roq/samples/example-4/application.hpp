/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_4 {

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &);
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
