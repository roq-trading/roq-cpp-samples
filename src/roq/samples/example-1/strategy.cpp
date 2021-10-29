/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-1/strategy.h"

#include "roq/logging.h"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_1 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connected> &event) {
  log::info("[{}:{}] connected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<Disconnected> &event) {
  log::info("[{}:{}] Disconnected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  log::info("[{}:{}] DownloadBegin={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  log::info("[{}:{}] DownloadEnd={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<GatewaySettings> &event) {
  log::info("[{}:{}] Settings={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<StreamStatus> &event) {
  log::info("[{}:{}] StreamStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<ExternalLatency> &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] ExternalLatency={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<RateLimitTrigger> &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] RateLimitTrigger={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] GatewayStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  log::info("[{}:{}] ReferenceData={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  log::info("[{}:{}] MarketStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<TopOfBook> &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] TopOfBook={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  // only verbose logging, see comment in header
  log::info<1>(
      "[{}:{}] MarketByPriceUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  // only verbose logging, see comment in header
  log::info<1>(
      "[{}:{}] MarketByOrderUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  // only verbose logging, see comment in header
  log::info<1>("[{}:{}] TradeSummary={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<CustomMetricsUpdate> &event) {
  // only verbose logging, see comment in header
  log::info<1>(
      "[{}:{}] CustomMetricsUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
