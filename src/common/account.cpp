/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "common/account.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <algorithm>

#define PREFIX "[" << _name << "] "

namespace examples {
namespace common {

namespace {
static std::vector<std::shared_ptr<Position> > create_positions(
    const Config& config,
    Account& account) {
  std::vector<std::shared_ptr<Position> > result;
  for (auto i = 0; i < config.instruments.size(); ++i) {
    const auto& instrument = config.instruments[i];
    // find long/short position limits
    double long_limit = 0.0, short_limit = 0.0;
    for (const auto& iter : instrument.accounts)
      if (iter.first == account.get_name()) {
        long_limit = iter.second.long_limit;
        short_limit = iter.second.short_limit;
        break;
      }
    result.emplace_back(
      std::make_shared<Position>(
        account,
        instrument.exchange,
        instrument.symbol,
        true,  // not (yet) supporting start of day position config
        long_limit,
        short_limit,
        0.0,
        0.0));
  }
  return result;
}
static std::unordered_map<std::string, std::shared_ptr<Position> >
create_positions_by_symbol(
    std::vector<std::shared_ptr<Position> >& positions) {
  std::unordered_map<std::string, std::shared_ptr<Position> > result;
  for (auto& iter : positions) {
    result.emplace(iter->get_symbol(), iter);
  }
  return result;
}
}  // namespace

Account::Account(
    roq::Client::Dispatcher& dispatcher,
    const std::string& gateway,
    const std::string& name,
    const Config& config)
    : _dispatcher(dispatcher),
      _gateway(gateway),
      _name(name),
      _positions(create_positions(config, *this)),
      _positions_by_symbol(create_positions_by_symbol(_positions)) {
}

std::shared_ptr<Position> Account::get_position(
    const std::string& exchange,
    const std::string& symbol) {
  return _positions_by_symbol.at(symbol);
}

void Account::on(const roq::TimerEvent&) {
  // FIXME(thraneh): test request timeout
}

void Account::on(const roq::DownloadBegin& download_begin) {
  LOG(INFO) << PREFIX "Account download starting...";
  _download = true;
  if (_order_manager_ready) {
    _order_manager_ready = false;
    LOG(INFO) << PREFIX "order_manager_ready=" <<
      (_order_manager_ready ? "true" : "false");
  }
  for (auto& position : _positions)
    position.reset();
}

void Account::on(const roq::DownloadEnd& download_end) {
  update_max_order_id(download_end.max_order_id);
  _download = false;
  LOG(INFO) << PREFIX "Account download completed";
}

void Account::on(const roq::PositionUpdate& position_update) {
  apply(
      position_update.exchange,
      position_update.symbol,
      [&](Position& position) {
          position.on(position_update); });
}

void Account::on(const roq::OrderUpdate& order_update) {
  apply(
      order_update.exchange,
      order_update.symbol,
      [&](Position& position) {
          position.on(order_update); });
}

void Account::on(const roq::TradeUpdate& trade_update) {
  apply(
      trade_update.exchange,
      trade_update.symbol,
      [&](Position& position) {
          position.on(trade_update); });
}

void Account::on(const roq::OrderManagerStatus& order_manager_status) {
  auto order_manager_ready =
      order_manager_status.status == roq::GatewayStatus::Ready;
  if (_order_manager_ready != order_manager_ready)
    _order_manager_ready = order_manager_ready;
  LOG(INFO) << PREFIX "order_manager_ready=" <<
    (_order_manager_ready ? "true" : "false");
}

void Account::update_max_order_id(uint32_t order_id) {
  auto max_order_id = std::max(order_id, _max_order_id);
  if (_max_order_id != max_order_id) {
    _max_order_id = max_order_id;
    LOG(INFO) << PREFIX "max_order_id=" << _max_order_id;
  }
}

bool Account::apply(
    const std::string& exchange,
    const std::string& symbol,
    std::function<void(Position&)> function) {
  auto iter = _positions_by_symbol.find(symbol);
  if (iter == _positions_by_symbol.end()) {
    LOG(WARNING) << "Got update for unknown "
      "exchange=\"" << exchange << "\", symbol=\"" << symbol << "\"";
    return false;
  }
  function(*(*iter).second);
  return true;
}

uint32_t Account::create_order(
    const std::string& exchange,
    const std::string& symbol,
    roq::Side side,
    double quantity,
    double price,
    roq::TimeInForce time_in_force,
    roq::PositionEffect position_effect,
    const std::string& order_template) {
  if (!_order_manager_ready) {
    LOG(WARNING) << PREFIX "Order manager is not in the ready state";
    throw roq::NotReady();
  }
  auto order_id = ++_max_order_id;
  LOG_IF(FATAL, is_order_live(order_id)) << PREFIX "Unexpected";
  roq::CreateOrder create_order {
    .account         = _name.c_str(),
    .order_id        = order_id,
    .exchange        = exchange.c_str(),
    .symbol          = symbol.c_str(),
    .side            = side,
    .quantity        = quantity,
    .order_type      = roq::OrderType::Limit,
    .limit_price     = price,
    .time_in_force   = time_in_force,
    .position_effect = position_effect,
    .order_template  = order_template.c_str(),
  };
  LOG(INFO) << PREFIX "create_order=" << create_order;
  _dispatcher.send(create_order, _gateway.c_str());
  _live_orders.insert(order_id);
  return order_id;
}

void Account::modify_order(
    uint32_t order_id,
    double quantity_change,
    double limit_price) {
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (!_order_manager_ready) {
    LOG(WARNING) << PREFIX "Order manager is not in the ready state";
    throw roq::NotReady();
  }
  if (_live_orders.find(order_id) == _live_orders.end())
    LOG(WARNING) << PREFIX "Not a live order (order_id=" << order_id << ")";
  roq::ModifyOrder modify_order {
    .account         = _name.c_str(),
    .order_id        = order_id,
    .quantity_change = quantity_change,
    .limit_price     = limit_price,
  };
  LOG(INFO) << PREFIX "modify_order=" << modify_order;
  _dispatcher.send(modify_order, _gateway.c_str());
}

void Account::cancel_order(uint32_t order_id) {
  if (is_order_live(order_id) == false)
    throw roq::OrderNotLive();
  if (!_order_manager_ready) {
    LOG(WARNING) << PREFIX "Order manager is not in the ready state";
    throw roq::NotReady();
  }
  if (_live_orders.find(order_id) == _live_orders.end())
    LOG(WARNING) << PREFIX "Not a live order (order_id=" << order_id << ")";
  roq::CancelOrder cancel_order {
    .account  = _name.c_str(),
    .order_id = order_id,
  };
  LOG(INFO) << PREFIX "cancel_order=" << cancel_order;
  _dispatcher.send(cancel_order, _gateway.c_str());
}

void Account::on(const roq::CreateOrderAck& create_order_ack) {
  auto iter = _live_orders.find(create_order_ack.order_id);
  if (create_order_ack.failure)
    _live_orders.erase(create_order_ack.order_id);
}

void Account::on(const roq::ModifyOrderAck& modify_order_ack) {
  auto iter = _live_orders.find(modify_order_ack.order_id);
  // there's nothing to do if a failure is being signaled...?
}

void Account::on(const roq::CancelOrderAck& cancel_order_ack) {
  auto iter = _live_orders.find(cancel_order_ack.order_id);
  // there's nothing to do if a failure is being signaled...?
}

bool Account::is_order_live(uint32_t order_id) const {
  return _live_orders.find(order_id) != _live_orders.end();
}

std::ostream& Account::write(std::ostream& stream) const {
  stream << "{"
    "name=\"" << _name << "\", "
    "download=" << (_download ? "true" : "false") << ", "
    "max_order_id=" << _max_order_id << ", "
    "ready=" << (_order_manager_ready ? "true" : "false") << ", "
    "positions={";
  bool first = true;
  for (const auto& iter : _positions_by_symbol) {
    stream << (first ? "" : ", ") << iter.first << "=" << *iter.second;
    first = false;
  }
  return stream << "}}";
}

}  // namespace common
}  // namespace examples
