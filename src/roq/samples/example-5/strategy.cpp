/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/example-5/strategy.h"

#include "roq/logging.h"

using namespace roq::literals;

namespace roq {
namespace samples {
namespace example_5 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher) {
}

void Strategy::operator()(const Event<Start> &) {
  // start thread
}

void Strategy::operator()(const Event<Stop> &) {
  // join thread
}

void Strategy::operator()(const Event<TopOfBook> &event) {
  log::info(
      "[{}:{}] TopOfBook={}"_fmt,
      event.message_info.source,
      event.message_info.source_name,
      event.value);
}

void Strategy::operator()(const Event<client::CustomMessage> &event) {
  log::info(
      "[{}:{}] TradeSummary={}"_fmt,
      event.message_info.source,
      event.message_info.source_name,
      event.value);
}

}  // namespace example_5
}  // namespace samples
}  // namespace roq
