/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/service.h"
#include "roq/span.h"

namespace roq {
namespace samples {
namespace example_5 {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view> &args);
  int main(int argc, char **argv) override;
};

}  // namespace example_5
}  // namespace samples
}  // namespace roq
