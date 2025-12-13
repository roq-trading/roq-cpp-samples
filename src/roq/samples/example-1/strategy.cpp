/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-1/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_1 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(Event<Connected> const &event) {
  log::info("[{}:{}] connected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
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

void Strategy::operator()(Event<GatewaySettings> const &event) {
  log::info("[{}:{}] Settings={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<StreamStatus> const &event) {
  log::info("[{}:{}] StreamStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<ExternalLatency> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] ExternalLatency={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<RateLimitTrigger> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] RateLimitTrigger={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<GatewayStatus> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] GatewayStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  log::info("[{}:{}] ReferenceData={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  log::info("[{}:{}] MarketStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] TopOfBook={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] MarketByPriceUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<MarketByOrderUpdate> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] MarketByOrderUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<TradeSummary> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] TradeSummary={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<CustomMetricsUpdate> const &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] CustomMetricsUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
