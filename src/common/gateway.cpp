/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/gateway.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

#include "common/instrument.h"

namespace examples {
namespace common {

namespace {
const char *TRADER = "Trader";  // FIXME(thraneh): introduce an enum for this!
}  // namespace

Gateway::Gateway(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& name)
    : _dispatcher(dispatcher),
      _name(name) {
}

void Gateway::on(const roq::MarketDataStatusEvent& event) {
  auto status = event.market_data_status.status;
  auto ready = status == roq::GatewayStatus::Ready;
  LOG(INFO) << "Market data " <<
    "is " << (ready ? "" : "not ") << "ready";
}

}  // namespace common
}  // namespace examples
