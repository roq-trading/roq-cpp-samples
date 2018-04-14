/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/gateway.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <gflags/gflags.h>

#include <algorithm>

namespace examples {
namespace common {

namespace {
const char *TRADER = "Trader";  // FIXME(thraneh): introduce an enum for this!
}  // namespace

// Currently only supporting a single gateway...
DEFINE_string(gtc_open, "gtc_open", "Order template.");
DEFINE_string(gtc_close, "gtc_close", "Order template.");

Gateway::Gateway(
    roq::Strategy::Dispatcher& dispatcher,
    const std::string& name)
    : _dispatcher(dispatcher),
      _name(name),
      _gtc_open(FLAGS_gtc_open),
      _gtc_close(FLAGS_gtc_close) {
}

void Gateway::on(const roq::DownloadBeginEvent& event) {
  // set download flag (block strategy from creating new orders)
  _download = true;
  LOG(INFO) << "download=" << (_download ? "true" : "false");
  // reset all variables tracking order management state
  _order_manager_ready = false;
  _order_traded_quantity.clear();
  // note! order_max_id should *NEVER* be reset
}

void Gateway::on(const roq::DownloadEndEvent& event) {
  // max order id
  update_max_order_id(event.download_end.max_order_id);
  // reset download flag
  _download = false;
  LOG(INFO) << "download=" << (_download ? "true" : "false");
}

void Gateway::on(const roq::GatewayStatusEvent& event) {
  const auto& gateway_status = event.gateway_status;
  // return early if it's not the gateway's order management status
  if (std::strcmp(TRADER, gateway_status.name) != 0)
    return;
  // ready?
  auto order_manager_ready = gateway_status.status == roq::GatewayState::Ready;
  if (_order_manager_ready != order_manager_ready) {
    _order_manager_ready = order_manager_ready;
    LOG(INFO) << "order_manager_ready=" << (_order_manager_ready ? "true" : "false");
  }
}

void Gateway::on(const roq::OrderUpdateEvent& event) {
  // max order id
  update_max_order_id(event.order_update.order_id);
}

void Gateway::update_max_order_id(uint32_t order_id) {
  auto max_order_id = std::max(order_id, _max_order_id);
  if (max_order_id != _max_order_id) {
    _max_order_id = max_order_id;
    LOG(INFO) << "max_order_id=" << _max_order_id;
  }
}

uint32_t Gateway::create_order(
    const std::string& exchange,
    const std::string& instrument,
    roq::TradeDirection direction,
    double quantity,
    double price,
    const std::string& order_template) {
  if (is_ready() == false)
    throw roq::NotReady();
  auto order_id = ++_max_order_id;
  roq::CreateOrder create_order {
    .order_id       = order_id,
    .order_template = order_template.c_str(),
    .exchange       = exchange.c_str(),
    .instrument     = instrument.c_str(),
    .direction      = direction,
    .quantity       = quantity,
    .limit_price    = price,
  };
  LOG(INFO) << "create_order=" << create_order;
  _dispatcher.send(create_order, _name.c_str());
  return order_id;
}

// The interface is generic an supposed to work for a multitude of
// gateways. The client therefore has no access to market specific
// fields. Those extra fields are only visible to the gateway. The
// client can, however, leverage such extra fields through templates.
// The gateway will ensure all order updates includes the original
// name of the template. The client can therefore safely compare
// the name of the template with that it used to create the order.
// Returns true if the order template is an "open".
// Returns false if the order template is a "close".
// Terminate program execution if the order template is unknown.
bool Gateway::parse_open_close(const char *order_template) const {
  if (_gtc_open.compare(order_template) == 0)
    return true;
  if (_gtc_close.compare(order_template) == 0)
    return false;
  LOG(FATAL) << "Unknown order_template=\"" << order_template << "\"";
}

double Gateway::get_fill_quantity(const roq::OrderUpdate& order_update) {
  auto& previous = _order_traded_quantity[order_update.order_id];
  auto result = std::max(0.0, order_update.traded_quantity - previous);
  previous = order_update.traded_quantity;
  return result;
}

}  // namespace common
}  // namespace examples
