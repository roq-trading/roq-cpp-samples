/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/event-log/processor.h"

#include <thread>

#include "roq/logging.h"

#include "roq/client.h"

using namespace std::chrono_literals;
using namespace roq::literals;

namespace roq {
namespace samples {
namespace event_log {

Processor::Processor(const std::string_view &path) : reader_(client::EventLogReaderFactory::create(path)) {
}

void Processor::dispatch() {
  while (!reader_->finished()) {
    auto updates = false;
    while (reader_->dispatch(*this)) {
      updates = true;
    }
    if (updates)
      continue;
    // if you get here you're probably tailing an event-log...
    // use filesystem monitoring to detect updates, or simply wait
    const auto delay = 10s;
    log::info("sleeping for {}..."_sv, delay);
    std::this_thread::sleep_for(delay);
  }
}

void Processor::operator()(const Event<GatewaySettings> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<StreamStatus> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<ExternalLatency> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<RateLimitTrigger> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<GatewayStatus> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<ReferenceData> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<MarketStatus> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<TopOfBook> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<MarketByPriceUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<MarketByOrderUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<TradeSummary> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<StatisticsUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<CreateOrder> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<ModifyOrder> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<CancelOrder> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<CancelAllOrders> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<OrderAck> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<OrderUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<TradeUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<PositionUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<FundsUpdate> &event) {
  log::info("{}"_sv, event);
}

void Processor::operator()(const Event<CustomMetricsUpdate> &event) {
  log::info("{}"_sv, event);
}

}  // namespace event_log
}  // namespace samples
}  // namespace roq
