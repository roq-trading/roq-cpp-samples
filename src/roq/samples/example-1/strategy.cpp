/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-1/strategy.h"

#include "roq/logging.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_1 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connection> &event) {
  LOG(INFO)
  (R"([{}:{}] Connection={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadBegin={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadEnd={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<GatewaySettings> &event) {
  LOG(INFO)
  (R"([{}:{}] Settings={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<StreamUpdate> &event) {
  LOG(INFO)
  (R"([{}:{}] StreamUpdate={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<ExternalLatency> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] ExternalLatency={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  LOG(INFO)
  (R"([{}:{}] ReferenceData={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] MarketStatus={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] MarketByPriceUpdate={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] MarketByOrderUpdate={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  // only verbose logging, see comment in header
  VLOG(1)
  (R"([{}:{}] TradeSummary={})"_fmt,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

}  // namespace example_1
}  // namespace samples
}  // namespace roq
