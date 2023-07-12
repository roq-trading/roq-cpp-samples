/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/tool.hpp"

namespace roq {
namespace samples {
namespace import {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

struct Application final : public Tool {
  using Tool::Tool;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace import
}  // namespace samples
}  // namespace roq
