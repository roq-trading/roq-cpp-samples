/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/algo-proto/route.hpp"

#include "roq/client.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace algo_proto {

// === HELPERS ===

namespace {
auto create_market_by_price = [](auto &exchange, auto &symbol) {
  return client::MarketByPriceFactory::create(exchange, symbol);
};
}

// === IMPLEMENTATION ===

Route::Route(uint32_t market_id, std::string_view const &exchange, std::string_view const &symbol)
    : market_{market_id, exchange, symbol, create_market_by_price} {
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
