/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <span>
#include <string>
#include <string_view>

#include "roq/cache/gateway.hpp"

#include "roq/algo/framework/dispatcher.hpp"
#include "roq/algo/framework/factory.hpp"
#include "roq/algo/framework/handler.hpp"
#include "roq/algo/framework/instrument.hpp"
#include "roq/algo/framework/market.hpp"
#include "roq/algo/framework/state.hpp"

namespace roq {
namespace algo {
namespace strategies {

class Factory final : public framework::Factory {
 public:
  Factory(
      const std::string_view &type,
      const std::span<const framework::Instrument> &,
      const std::span<const framework::Market *> &,
      const std::span<const cache::Gateway> &);

  Factory(Factory &&) = default;
  Factory(const Factory &) = delete;

  operator const framework::State &() const override { return state_; }

  std::unique_ptr<framework::Handler> create(
      framework::Dispatcher &, const std::string_view &routing_id, const CreateOrder &) override;

 private:
  const std::string type_;
  const framework::State state_;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
