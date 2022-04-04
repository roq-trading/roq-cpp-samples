/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace republish {

// application

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const std::span<std::string_view> &args);
  int main(int argc, char **argv) override;
};

}  // namespace republish
}  // namespace samples
}  // namespace roq
