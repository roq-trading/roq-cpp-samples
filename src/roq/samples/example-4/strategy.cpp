/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-4/strategy.h"

#include "roq/logging.h"

using namespace std::literals;  // NOLINT

namespace roq {
namespace samples {
namespace example_4 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Connection> &event) {
  LOG(INFO)
  (R"([{}:{}] Connection={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadBegin={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  LOG(INFO)
  (R"([{}:{}] DownloadEnd={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketDataStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] MarketDataStatus={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<OrderManagerStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] OrderManagerStatus={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  LOG(INFO)
  (R"([{}:{}] ReferenceData={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  LOG(INFO)
  (R"([{}:{}] MarketStatus={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  VLOG(1)
  (R"([{}:{}] MarketByPriceUpdate={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  VLOG(1)
  (R"([{}:{}] MarketByOrderUpdate={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  VLOG(1)
  (R"([{}:{}] TradeSummary={})"sv,
   event.message_info.source,
   event.message_info.source_name,
   event.value);
}

}  // namespace example_4
}  // namespace samples
}  // namespace roq
