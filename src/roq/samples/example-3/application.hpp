/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_3 {

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(std::span<std::string_view> const &args);

  int main(int argc, char **argv) override;
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq
