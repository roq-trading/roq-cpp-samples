/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-6/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_6 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Timer> &event) {
  check(event);
}

void Strategy::operator()(const Event<Connected> &event) {
  check(event);
}

void Strategy::operator()(const Event<Disconnected> &event) {
  check(event);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  check(event);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  check(event);
}

void Strategy::operator()(const Event<GatewaySettings> &event) {
  check(event);
}

void Strategy::operator()(const Event<StreamStatus> &event) {
  check(event);
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  check(event);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  check(event);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  check(event);
}

void Strategy::operator()(const Event<TopOfBook> &event) {
  check(event);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<MarketByOrderUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<TradeSummary> &event) {
  check(event);
}

void Strategy::operator()(const Event<StatisticsUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<OrderAck> &event) {
  check(event);
}

void Strategy::operator()(const Event<OrderUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<TradeUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<PositionUpdate> &event) {
  check(event);
}

void Strategy::operator()(const Event<FundsUpdate> &event) {
  check(event);
}

template <typename T>
void Strategy::check(const Event<T> &event) {
  log::info("event={}"sv, event);
  auto receive_time = event.message_info.receive_time;
  log::debug<4>(
      "prev={}, next={}, delta={}, source={}"sv,
      receive_time,
      prev_receive_time_,
      receive_time - prev_receive_time_,
      event.message_info.source);
  if (receive_time < prev_receive_time_)
    log::fatal("Discovered a time machine (receive_time={}, prev_receive_time={})"sv, receive_time, prev_receive_time_);
  prev_receive_time_ = receive_time;
}

}  // namespace example_6
}  // namespace samples
}  // namespace roq
