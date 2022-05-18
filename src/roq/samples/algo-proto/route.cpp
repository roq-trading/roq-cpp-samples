/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/algo-proto/route.hpp"

#include "roq/client.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace algo_proto {

Route::Route(uint32_t market_id, std::string_view const &exchange, std::string_view const &symbol)
    : market_(market_id, exchange, symbol, [](auto &exchange, auto &symbol) {
        return client::MarketByPriceFactory::create(exchange, symbol);
      }) {
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
