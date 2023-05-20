/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/python/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace python {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_{dispatcher}, producer_{dispatcher} {
}

void Strategy::operator()(Event<Start> const &event) {
  producer_(event);
}

void Strategy::operator()(Event<Stop> const &event) {
  producer_(event);
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  log::info("[{}:{}] TopOfBook={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

void Strategy::operator()(Event<client::CustomMessage> const &event) {
  log::info("[{}:{}] CustomMessage={}"sv, event.message_info.source, event.message_info.source_name, event.value);
}

}  // namespace python
}  // namespace samples
}  // namespace roq
