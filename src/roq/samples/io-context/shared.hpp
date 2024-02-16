/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/container.hpp"

namespace roq {
namespace samples {
namespace io_context {

struct Shared final {
  utils::unordered_set<uint64_t> sessions_to_remove;
  utils::unordered_set<std::string> symbols;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
