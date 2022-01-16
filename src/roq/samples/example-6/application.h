/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.h"

namespace roq {
namespace samples {
namespace example_6 {

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const std::span<std::string_view> &args);

  int main(int argc, char **argv) override;
};

}  // namespace example_6
}  // namespace samples
}  // namespace roq
