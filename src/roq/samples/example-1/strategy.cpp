/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-1/strategy.h"

#include "roq/logging.h"

namespace roq {
namespace samples {
namespace example_1 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connection> &event) {
  LOG(INFO)
  (R"([{}:{}] Connection={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadBegin={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadEnd={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<GatewaySettings> &event) {
  LOG(INFO)
  (R"([{}:{}] Settings={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketDataStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] MarketDataStatus={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<OrderManagerStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] OrderManagerStatus={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  LOG(INFO)
  (R"([{}:{}] ReferenceData={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] MarketStatus={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] MarketByPriceUpdate={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] MarketByOrderUpdate={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] TradeSummary={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<ExternalLatency> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] ExternalLatency={})",
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
