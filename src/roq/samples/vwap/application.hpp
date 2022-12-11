/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/tool.hpp"

namespace roq {
namespace samples {
namespace vwap {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

class Application final : public Tool {
 public:
  using Tool::Tool;

 protected:
  int main_helper(std::span<std::string_view> const &args);
  int main(int argc, char **argv) override;
};

}  // namespace vwap
}  // namespace samples
}  // namespace roq
