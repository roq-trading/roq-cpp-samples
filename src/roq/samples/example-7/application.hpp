/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace example_7 {

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(std::span<std::string_view> const &args);
  int main(int argc, char **argv) override;
};

}  // namespace example_7
}  // namespace samples
}  // namespace roq
