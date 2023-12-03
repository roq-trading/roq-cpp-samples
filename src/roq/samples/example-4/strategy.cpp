/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/example-4/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_4 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_{dispatcher} {
}

void Strategy::operator()(Event<Connected> const &event) {
  log::info("[{}:{}] Connected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  log::info("[{}:{}] Disconnected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  log::info("[{}:{}] DownloadBegin={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  log::info("[{}:{}] DownloadEnd={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<GatewayStatus> const &event) {
  log::info("[{}:{}] GatewayStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  log::info("[{}:{}] ReferenceData={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  log::info("[{}:{}] MarketStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  log::info<1>(
      "[{}:{}] MarketByPriceUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketByOrderUpdate> const &event) {
  log::info<1>(
      "[{}:{}] MarketByOrderUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<TradeSummary> const &event) {
  log::info<1>("[{}:{}] TradeSummary={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
