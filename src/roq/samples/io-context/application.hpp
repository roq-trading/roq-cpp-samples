/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/service.hpp"

#include "roq/io/context.hpp"

namespace roq {
namespace samples {
namespace io_context {

// application

struct Application final : public Service {
  using Service::Service;

 protected:
  int main_helper(std::span<std::string_view> const &args, io::Context &);
  int main(int argc, char **argv) override;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
