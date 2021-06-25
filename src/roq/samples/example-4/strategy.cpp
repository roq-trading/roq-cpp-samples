/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-4/strategy.h"

#include "roq/logging.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_4 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connected> &event) {
  log::info("[{}:{}] Connected={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<Disconnected> &event) {
  log::info("[{}:{}] Disconnected={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  log::info("[{}:{}] DownloadBegin={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  log::info("[{}:{}] DownloadEnd={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  log::info("[{}:{}] GatewayStatus={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  log::info("[{}:{}] ReferenceData={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  log::info("[{}:{}] MarketStatus={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  log::info<1>(
      "[{}:{}] MarketByPriceUpdate={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  log::info<1>(
      "[{}:{}] MarketByOrderUpdate={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  log::info<1>("[{}:{}] TradeSummary={}"_sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
