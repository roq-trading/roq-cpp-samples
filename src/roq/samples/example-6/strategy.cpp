/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/samples/example-6/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_6 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_{dispatcher} {
}

void Strategy::operator()(Event<Timer> const &event) {
  check(event);
}

void Strategy::operator()(Event<Connected> const &event) {
  check(event);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  check(event);
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  check(event);
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  check(event);
}

void Strategy::operator()(Event<GatewaySettings> const &event) {
  check(event);
}

void Strategy::operator()(Event<StreamStatus> const &event) {
  check(event);
}

void Strategy::operator()(Event<GatewayStatus> const &event) {
  check(event);
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  check(event);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  check(event);
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  check(event);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<MarketByOrderUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<TradeSummary> const &event) {
  check(event);
}

void Strategy::operator()(Event<StatisticsUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<OrderAck> const &event) {
  check(event);
}

void Strategy::operator()(Event<OrderUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<TradeUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<PositionUpdate> const &event) {
  check(event);
}

void Strategy::operator()(Event<FundsUpdate> const &event) {
  check(event);
}

template <typename T>
void Strategy::check(Event<T> const &event) {
  log::info("event={}"sv, event);
  auto receive_time = event.message_info.receive_time;
  log::debug<4>("prev={}, next={}, delta={}, source={}"sv, receive_time, prev_receive_time_, receive_time - prev_receive_time_, event.message_info.source);
  if (receive_time < prev_receive_time_) {
    log::fatal("Discovered a time machine (receive_time={}, prev_receive_time={})"sv, receive_time, prev_receive_time_);
  }
  prev_receive_time_ = receive_time;
}

}  // namespace example_6
}  // namespace samples
}  // namespace roq
