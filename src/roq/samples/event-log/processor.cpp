/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/event-log/processor.hpp"

#include <thread>

#include "roq/logging.hpp"

#include "roq/client.hpp"

using namespace std::chrono_literals;
using namespace std::literals;

namespace roq {
namespace samples {
namespace event_log {

// === IMPLEMENTATION ===

Processor::Processor(std::string_view const &path) : reader_{client::EventLogReaderFactory::create(path)} {
}

void Processor::dispatch() {
  while (!(*reader_).finished()) {
    auto updates = false;
    while ((*reader_).dispatch(*this)) {
      updates = true;
    }
    if (updates)
      continue;
    // if you get here you're probably tailing an event-log...
    // use filesystem monitoring to detect updates, or simply wait
    auto const delay = 10s;
    log::info("sleeping for {}..."sv, delay);
    std::this_thread::sleep_for(delay);
  }
}

void Processor::operator()(Event<GatewaySettings> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<StreamStatus> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<ExternalLatency> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<RateLimitTrigger> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<GatewayStatus> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<ReferenceData> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<MarketStatus> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<TopOfBook> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<MarketByPriceUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<MarketByOrderUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<TradeSummary> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<StatisticsUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CreateOrder> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<ModifyOrder> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CancelOrder> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CancelAllOrders> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CancelAllOrdersAck> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<OrderAck> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<OrderUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<TradeUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<PositionUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<FundsUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CustomMetricsUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<CustomMatrixUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<ParametersUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<PortfolioUpdate> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<RiskLimits> const &event) {
  log::info("{}"sv, event);
}

void Processor::operator()(Event<RiskLimitsUpdate> const &event) {
  log::info("{}"sv, event);
}

}  // namespace event_log
}  // namespace samples
}  // namespace roq
