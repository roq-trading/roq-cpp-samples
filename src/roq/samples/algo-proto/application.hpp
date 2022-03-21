/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.hpp"

namespace roq {
namespace samples {
namespace algo_proto {

class Application final : public roq::Service {
 public:
  using Service::Service;

 protected:
  int main(int argc, char **argv) override;

  int main_helper(const std::span<std::string_view> &args);
};

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
