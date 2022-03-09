/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-5/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_5 {

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_(dispatcher), producer_(dispatcher) {
}

void Strategy::operator()(const Event<Start> &event) {
  producer_(event);
}

void Strategy::operator()(const Event<Stop> &event) {
  producer_(event);
}

void Strategy::operator()(const Event<TopOfBook> &event) {
  log::info("[{}:{}] TopOfBook={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(const Event<client::CustomMessage> &event) {
  log::info("[{}:{}] CustomMessage={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace example_5
}  // namespace samples
}  // namespace roq
