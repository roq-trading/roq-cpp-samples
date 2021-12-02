/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/service.h"
#include "roq/span.h"

namespace roq {
namespace samples {
namespace example_4 {

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view> &args);
  int main(int argc, char **argv) override;
};

}  // namespace example_4
}  // namespace samples
}  // namespace roq
