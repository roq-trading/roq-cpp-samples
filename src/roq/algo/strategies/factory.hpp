/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

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

struct Factory final : public framework::Factory {
  Factory(
      std::string_view const &type,
      std::span<framework::Instrument const> const &,
      std::span<framework::Market const *> const &,
      std::span<cache::Gateway const> const &);

  Factory(Factory &&) = default;
  Factory(Factory const &) = delete;

  operator framework::State const &() const override { return state_; }

  std::unique_ptr<framework::Handler> create(framework::Dispatcher &, std::string_view const &routing_id, CreateOrder const &) override;

 private:
  std::string const type_;
  framework::State const state_;
};

}  // namespace strategies
}  // namespace algo
}  // namespace roq
