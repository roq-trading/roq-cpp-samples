/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_set.h>

namespace roq {
namespace samples {
namespace io_context {

struct Shared final {
  absl::flat_hash_set<uint64_t> sessions_to_remove;
};

}  // namespace io_context
}  // namespace samples
}  // namespace roq
