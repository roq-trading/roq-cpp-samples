/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/metrics/strategy.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace metrics {

// === CONSTANTS ===

namespace {
// note! labels are optional
auto const LABELS = R"(foo="bar", baz="123")"sv;  // note! all values should be quoted
}  // namespace

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher) : dispatcher_{dispatcher}, counter_{LABELS}, histogram_{LABELS} {
}

void Strategy::operator()(Event<Disconnected> const &) {
  ++counter_;
}

void Strategy::operator()(Event<ExternalLatency> const &event) {
  histogram_.update(event.value.latency.count());
}

// note! the writer is called from another thread -- always only use atomic variables!
void Strategy::operator()(roq::metrics::Writer &writer) const {
  writer  //
      .write(counter_, roq::metrics::COUNTER)
      .write(histogram_, roq::metrics::PROFILE);
}

}  // namespace metrics
}  // namespace samples
}  // namespace roq
