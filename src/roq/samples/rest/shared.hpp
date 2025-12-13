/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/container.hpp"

namespace roq {
namespace samples {
namespace rest {

struct Shared final {
  utils::unordered_set<uint64_t> sessions_to_remove;
  utils::unordered_set<std::string> symbols;
};

}  // namespace rest
}  // namespace samples
}  // namespace roq
