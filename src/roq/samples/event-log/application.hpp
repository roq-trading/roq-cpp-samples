/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/tool.hpp"

namespace roq {
namespace samples {
namespace event_log {

// Service is a base class used to initialize the environment
// it is not required, only for your convenience

struct Application final : public Tool {
  using Tool::Tool;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace event_log
}  // namespace samples
}  // namespace roq
