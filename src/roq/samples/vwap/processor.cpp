/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/vwap/processor.hpp"

#include <utility>

#include "roq/logging.hpp"

#include "roq/client.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace vwap {

// === IMPLEMENTATION ===

Processor::Processor(Settings const &settings) : settings_{settings} {
}

void Processor::dispatch(Settings const &settings, std::string_view const &path) {
  Processor processor{settings};
  auto reader = client::EventLogReaderFactory::create(path);
  (*reader).dispatch(processor);
}

void Processor::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  auto &market_by_price = get_market_by_price(market_by_price_update);
  market_by_price(market_by_price_update);
  auto layer = market_by_price.compute_vwap(settings_.quantity);
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
