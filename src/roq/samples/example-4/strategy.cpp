/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-4/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_4 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connected> &event) {
  log::info("[{}:{}] Connected={}"sv, event.message_info.source, event.message_info.source_name, event.value);
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

void Strategy::operator()(const Event<GatewayStatus> &event) {
  log::info("[{}:{}] GatewayStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  log::info("[{}:{}] ReferenceData={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  log::info("[{}:{}] MarketStatus={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  log::info<1>(
      "[{}:{}] MarketByPriceUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  log::info<1>(
      "[{}:{}] MarketByOrderUpdate={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  log::info<1>("[{}:{}] TradeSummary={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
