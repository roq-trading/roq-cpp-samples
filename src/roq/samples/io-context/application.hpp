/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <span>

#include "roq/service.hpp"

#include "roq/io/context.hpp"

namespace roq {
namespace samples {
namespace io_context {

// application

class Application final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(std::span<std::string_view> const &args, io::Context &);
  int main(int argc, char **argv) override;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
