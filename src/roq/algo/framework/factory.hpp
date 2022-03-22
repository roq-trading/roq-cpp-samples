/* Copyright (c) 2017-2022, Hans Erik Thrane */

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

class Factory {
 public:
  virtual ~Factory() = default;

  virtual operator const framework::State &() const = 0;
  virtual std::unique_ptr<Handler> create(Dispatcher &, const std::string_view &routing_id, const CreateOrder &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
