/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/algo/strategies/factory.hpp"

#include <memory>

#include "roq/algo/strategies/spread.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace strategies {

// === IMPLEMENTATION ===

Factory::Factory(
    std::string_view const &type,
    std::span<framework::Instrument const> const &instruments,
    std::span<framework::Market const *> const &markets,
    std::span<cache::Gateway const> const &gateways)
    : type_{type}, state_{instruments, markets, gateways} {
}

std::unique_ptr<framework::Handler> Factory::create(framework::Dispatcher &dispatcher, std::string_view const &routing_id, CreateOrder const &create_order) {
  if (type_.compare("spread"sv) == 0)
    return std::make_unique<Spread>(dispatcher, state_, routing_id, create_order);
  throw RuntimeError{R"(Unknown type="{}")"sv, type_};
}

}  // namespace strategies
}  // namespace algo
}  // namespace roq
