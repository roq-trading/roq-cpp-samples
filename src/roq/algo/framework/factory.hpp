/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <memory>
#include <string_view>

#include "roq/api.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/handler.hpp"
#include "roq/algo/framework/state.hpp"

namespace roq {
namespace algo {
namespace framework {

struct Factory {
  virtual ~Factory() = default;

  virtual operator framework::State const &() const = 0;
  virtual std::unique_ptr<Handler> create(Dispatcher &, std::string_view const &routing_id, CreateOrder const &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
