/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/algo/strategies/factory.hpp"

#include <memory>

#include "roq/algo/strategies/spread.hpp"

using namespace std::literals;

namespace roq {
namespace algo {
namespace strategies {

Factory::Factory(
    const std::string_view &type,
    const std::span<const framework::Instrument> &instruments,
    const std::span<const framework::Market *> &markets,
    const std::span<const cache::Gateway> &gateways)
    : type_(type), state_(instruments, markets, gateways) {
}

std::unique_ptr<framework::Handler> Factory::create(
    framework::Dispatcher &dispatcher, const std::string_view &routing_id, const CreateOrder &create_order) {
  if (type_.compare("spread"sv) == 0)
    return std::make_unique<Spread>(dispatcher, state_, routing_id, create_order);
  throw RuntimeError(R"(Unknown type="{}")"sv, type_);
}

}  // namespace strategies
}  // namespace algo
}  // namespace roq
