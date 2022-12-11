/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/vwap/processor.hpp"

#include <utility>

#include "roq/logging.hpp"

#include "roq/client.hpp"

#include "roq/samples/vwap/flags/flags.hpp"

using namespace std::chrono_literals;
using namespace std::literals;

namespace roq {
namespace samples {
namespace vwap {

// === IMPLEMENTATION ===

void Processor::dispatch(std::string_view const &path) {
  Processor processor;
  auto reader = client::EventLogReaderFactory::create(path);
  (*reader).dispatch(processor);
}

void Processor::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  auto &market_by_price = get_market_by_price(market_by_price_update);
  market_by_price(market_by_price_update);
  auto layer = market_by_price.compute_vwap(flags::Flags::quantity());
  fmt::print(
      "{},{},{},{},{},{}\n"sv,
      message_info.receive_time,
      market_by_price_update.symbol,
      layer.bid_price,
      layer.bid_quantity,
      layer.ask_price,
      layer.ask_quantity);
}

cache::MarketByPrice &Processor::get_market_by_price(auto const &value) {
  auto iter = market_by_price_.find(value.symbol);
  if (iter == std::end(market_by_price_)) {
    auto market_by_price = client::MarketByPriceFactory::create(value.exchange, value.symbol);
    iter = market_by_price_.emplace(value.symbol, std::move(market_by_price)).first;
  }
  return *(*iter).second;
}

}  // namespace vwap
}  // namespace samples
}  // namespace roq
