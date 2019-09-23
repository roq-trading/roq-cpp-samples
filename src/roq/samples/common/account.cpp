/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/common/account.h"

#include <algorithm>

#include "roq/api.h"
#include "roq/logging.h"
#include "roq/stream.h"

#define PREFIX "[" << _name << "] "

namespace roq {
namespace samples {
namespace common {

namespace {
static auto create_positions(
    const Config& config,
    Account& account) {
  std::unordered_map<std::string, Position> result;
  for (const auto& instrument : config.instruments) {
    // find long/short position limits
    double long_limit = 0.0, short_limit = 0.0;
    for (const auto& iter : instrument.accounts)
      if (iter.first == account.get_name()) {
        long_limit = iter.second.long_limit;
        short_limit = iter.second.short_limit;
        break;
      }
    result.emplace(
        instrument.symbol,
        Position(
          account,
          long_limit,
          short_limit,
          0.0,
          0.0));
  }
  return result;
}
}  // namespace

namespace {
template <typename F>
inline bool apply(
  std::unordered_map<std::string, Position>& positions,
  const std::string_view& exchange,
  const std::string_view& symbol,
  F function) {
  auto iter = positions.find(std::string(symbol));  // FIXME(thraneh): alloc
  if (iter == positions.end()) {
    LOG(WARNING) << "Got update for unknown "
    "exchange=\"" << exchange << "\", symbol=\"" << symbol << "\"";
    return false;
  }
  function((*iter).second);
  return true;
}
}  // namespace

Account::Account(
    client::Dispatcher& dispatcher,
    const std::string_view& name,
    const Config& config)
    : _dispatcher(dispatcher),
      _name(name),
      _positions(create_positions(config, *this)) {
}

void Account::on(const TimerEvent&) {
  // TODO(thraneh): request timeout
}

void Account::on(const DownloadBegin&) {
  LOG(INFO) << PREFIX "Account download starting...";
  _download = true;
  if (_order_manager_ready) {
    _order_manager_ready = false;
    LOG(INFO) << PREFIX "order_manager_ready=" <<
      (_order_manager_ready ? "true" : "false");
  }
  for (auto& iter : _positions)
    iter.second.reset();
}

void Account::on(const DownloadEnd& download_end) {
  update_max_order_id(download_end.max_order_id);
  _download = false;
  LOG(INFO) << PREFIX "Account download completed";
}

void Account::on(const PositionUpdate& position_update) {
  apply(
      _positions,
      position_update.exchange,
      position_update.symbol,
      [&](Position& position) {
    position.on(position_update, _download); });
}

void Account::on(const OrderUpdate&) {
  // don't process -- prefer trade updates
  // TODO(thraneh): this is probably not universally true...
  // there was a reason why this wasn't prefered for CFFEX/Femas
}

void Account::on(const TradeUpdate& trade_update) {
  apply(
      _positions,
      trade_update.exchange,
      trade_update.symbol,
      [&](Position& position) {
    position.on(trade_update, _download); });
}

void Account::on(const OrderManagerStatus& order_manager_status) {
  auto order_manager_ready =
    order_manager_status.status == GatewayStatus::READY;
  if (_order_manager_ready != order_manager_ready)
    _order_manager_ready = order_manager_ready;
  LOG(INFO) << PREFIX "order_manager_ready=" <<
    (_order_manager_ready ? "true" : "false");
}

uint32_t Account::create_order(
    const std::string_view& exchange,
    const std::string_view& symbol,
    Side side,
    double quantity,
    double price,
    TimeInForce time_in_force,
    PositionEffect position_effect,
    const std::string_view& order_template) {
  if (!_order_manager_ready) {
    throw NotReady("Order manager is not in the ready state");
  }
  auto order_id = ++_max_order_id;
  LOG_IF(FATAL, is_order_live(order_id)) << PREFIX "Unexpected";
  CreateOrder create_order {
    .account         = _name.data(),
    .order_id        = order_id,
    .exchange        = exchange.data(),
    .symbol          = symbol.data(),
    .side            = side,
    .quantity        = quantity,
    .order_type      = OrderType::LIMIT,
    .limit_price     = price,
    .time_in_force   = time_in_force,
    .position_effect = position_effect,
    .order_template  = order_template.data(),
  };
  VLOG(1) << PREFIX "create_order=" << create_order;
  auto source = 0;  // FIXME(thraneh): relax this restriction
  _dispatcher.send(create_order, source);
  _live_orders.insert(order_id);
  return order_id;
}

void Account::modify_order(
    uint32_t order_id,
    double quantity_change,
    double limit_price) {
  if (!_order_manager_ready) {
    throw NotReady("Order manager is not in the ready state");
  }
  if (!is_order_live(order_id)) {
    throw OrderNotLive("Order is not live");
  }
  ModifyOrder modify_order {
    .account         = _name.c_str(),
    .order_id        = order_id,
    .quantity_change = quantity_change,
    .limit_price     = limit_price,
  };
  VLOG(1) << PREFIX "modify_order=" << modify_order;
  auto source = 0;  // FIXME(thraneh): relax this restriction
  _dispatcher.send(modify_order, source);
}

void Account::cancel_order(uint32_t order_id) {
  if (!_order_manager_ready) {
    throw NotReady("Order manager is not in the ready state");
  }
  if (!is_order_live(order_id)) {
    throw OrderNotLive("Order is not live");
  }
  CancelOrder cancel_order {
    .account  = _name.c_str(),
    .order_id = order_id,
  };
  VLOG(1) << PREFIX "cancel_order=" << cancel_order;
  auto source = 0;  // FIXME(thraneh): relax this restriction
  _dispatcher.send(cancel_order, source);
}

void Account::on(const CreateOrderAck& create_order_ack) {
  if (create_order_ack.failure) {
    // the order was not accepted: we should remove it again
    _live_orders.erase(create_order_ack.order_id);
  }
}

void Account::on(const ModifyOrderAck&) {
  // there's not much to do here -- an order update should
  // arrive asynchronously with new order attributes
}

void Account::on(const CancelOrderAck&) {
  // there's not much to do here -- an order update should
  // arrive asynchronously with new order attributes
}

void Account::update_max_order_id(uint32_t order_id) {
  auto max_order_id = std::max(order_id, _max_order_id);
  if (_max_order_id != max_order_id) {
    _max_order_id = max_order_id;
    LOG(INFO) << PREFIX "max_order_id=" << _max_order_id;
  }
}

std::ostream& Account::write(std::ostream& stream) const {
  return stream << "{"
    "name=\"" << _name << "\", "
    "download=" << (_download ? "true" : "false") << ", "
    "max_order_id=" << _max_order_id << ", "
    "ready=" << (_order_manager_ready ? "true" : "false") << ", "
    "positions=" << roq::join(_positions) <<
    "}";
}

}  // namespace common
}  // namespace samples
}  // namespace roq
