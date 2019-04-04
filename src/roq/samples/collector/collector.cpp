/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/collector/collector.h"

#include "gflags/gflags.h"

#include "roq/logging.h"
#include "roq/stream.h"

DEFINE_string(
    symbols,
    "",
    "symbols, one or more comma-separated lists prefixed "
    "with exchange, e.g. \"E1:S1,S2;E2:S3\")");

namespace roq {
namespace samples {
namespace collector {

Collector::Collector(roq::client::Dispatcher& dispatcher)
    : _dispatcher(dispatcher) {
}

void Collector::on(const roq::MarketByPriceEvent& event) {
  LOG(INFO) << "MarketByPriceEvent=" << event;
}

}  // namespace collector
}  // namespace samples
}  // namespace roq
