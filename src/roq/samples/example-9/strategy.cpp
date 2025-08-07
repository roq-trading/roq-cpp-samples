/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/example-9/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_9 {

// === HELPERS ===

namespace {
template <typename T>
void debug_print(Event<T> const &event) {
  auto &[message_info, value] = event;
  log::debug("[{}:{}] {}={}"sv, message_info.source, message_info.source_name, get_name<T>(), value);
}
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher, Settings const &settings) : dispatcher_{dispatcher}, time_series_{settings.max_history} {
}

void Strategy::operator()(Event<Timer> const &) {
  // bars are only produced when there are events
  // this is the place to "fill in the blanks", if a regular action is required
}

void Strategy::operator()(Event<Connected> const &event) {
  debug_print(event);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  debug_print(event);
}

void Strategy::operator()(Event<Ready> const &event) {
  debug_print(event);
}

void Strategy::operator()(Event<TimeSeriesUpdate> const &event) {
  debug_print(event);
  auto &[message_info, time_series_update] = event;
  // filter
  if (time_series_update.origin != Origin::GATEWAY || time_series_update.type != TimeSeriesType::TRADES || time_series_update.frequency != 1min) {
    return;
  }
  // process (on change)
  if (time_series_(event)) {
    process(time_series_, time_series_update.update_type);
  }
}

void Strategy::process(std::span<Bar const> const &history, UpdateType update_type) {
  log::info("update_type={}"sv, update_type);
  for (size_t i = 0; i < std::size(history); ++i) {
    auto &bar = history[i];
    log::info("[{}] {}"sv, i, bar);
  }
}

}  // namespace example_9
}  // namespace samples
}  // namespace roq
