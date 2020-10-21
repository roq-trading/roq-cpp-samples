/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/samples/example-3/instrument.h"

#include "roq/logging.h"

#include "roq/client.h"

#include "roq/samples/example-3/utilities.h"

namespace roq {
namespace samples {
namespace example_3 {

constexpr double TOLERANCE = 1.0e-10;

Instrument::Instrument(
    const std::string_view &exchange,
    const std::string_view &symbol,
    const std::string_view &account)
    : _exchange(exchange), _symbol(symbol), _account(account),
      _depth_builder(client::DepthBuilderFactory::create(symbol, _depth)) {
}

double Instrument::position() const {
  return (std::isnan(_long_position) ? double{0.0} : _long_position) -
         (std::isnan(_short_position) ? double{0.0} : _short_position);
}

bool Instrument::can_trade(Side side) const {
  switch (side) {
    case Side::BUY:
      return position() <= TOLERANCE;
    case Side::SELL:
      return position() >= -TOLERANCE;
    default:
      assert(false);  // unexpected / why call this function at all?
      return false;
  }
}

void Instrument::operator()(const Connection &connection) {
  if (update(_connection_status, connection.status)) {
    LOG(INFO)
    (R"([{}:{}] connection_status={})", _exchange, _symbol, _connection_status);
    check_ready();
  }
  switch (_connection_status) {
    case ConnectionStatus::UNDEFINED:
      LOG(FATAL)("Unexpected");
      break;
    case ConnectionStatus::CONNECTED:
      // nothing to do for this implementation
      break;
    case ConnectionStatus::DISCONNECTED:
      // reset all cached state - await download upon reconnection
      reset();
      break;
  }
}

void Instrument::operator()(const DownloadBegin &download_begin) {
  if (download_begin.account.empty() == false) return;
  assert(_download == false);
  _download = true;
  LOG(INFO)(R"([{}:{}] download={})", _exchange, _symbol, _download);
}

void Instrument::operator()(const DownloadEnd &download_end) {
  if (download_end.account.empty() == false) return;
  assert(_download == true);
  _download = false;
  LOG(INFO)(R"([{}:{}] download={})", _exchange, _symbol, _download);
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketDataStatus &market_data_status) {
  // update our cache
  if (update(_market_data_status, market_data_status.status)) {
    LOG(INFO)
    (R"([{}:{}] market_data_status={})",
     _exchange,
     _symbol,
     _market_data_status);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const OrderManagerStatus &order_manager_status) {
  assert(_account.compare(order_manager_status.account) == 0);
  // update our cache
  if (update(_order_manager_status, order_manager_status.status)) {
    LOG(INFO)
    (R"([{}:{}] order_manager_status={})",
     _exchange,
     _symbol,
     _order_manager_status);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const ReferenceData &reference_data) {
  assert(_exchange.compare(reference_data.exchange) == 0);
  assert(_symbol.compare(reference_data.symbol) == 0);
  // update the depth builder
  _depth_builder->update(reference_data);
  // update our cache
  if (update(_tick_size, reference_data.tick_size)) {
    LOG(INFO)(R"([{}:{}] tick_size={})", _exchange, _symbol, _tick_size);
  }
  if (update(_min_trade_vol, reference_data.min_trade_vol)) {
    LOG(INFO)
    (R"([{}:{}] min_trade_vol={})", _exchange, _symbol, _min_trade_vol);
  }
  if (update(_multiplier, reference_data.multiplier)) {
    LOG(INFO)(R"([{}:{}] multiplier={})", _exchange, _symbol, _multiplier);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketStatus &market_status) {
  assert(_exchange.compare(market_status.exchange) == 0);
  assert(_symbol.compare(market_status.symbol) == 0);
  // update our cache
  if (update(_trading_status, market_status.trading_status)) {
    LOG(INFO)
    (R"([{}:{}] trading_status={})", _exchange, _symbol, _trading_status);
  }
  // update the ready flag
  check_ready();
}

void Instrument::operator()(const MarketByPriceUpdate &market_by_price_update) {
  assert(_exchange.compare(market_by_price_update.exchange) == 0);
  assert(_symbol.compare(market_by_price_update.symbol) == 0);
  LOG_IF(INFO, _download)(R"(MarketByPriceUpdate={})", market_by_price_update);
  // update depth
  // note!
  //   market by price only gives you *changes*.
  //   you will most likely want to use the the price to look up
  //   the relative position in an order book and then modify the
  //   liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  _depth_builder->update(market_by_price_update);
  VLOG(1)(R"([{}:{}] depth=[{}])", _exchange, _symbol, fmt::join(_depth, ", "));
  validate(_depth);
}

void Instrument::operator()(const MarketByOrderUpdate &market_by_order_update) {
  assert(_exchange.compare(market_by_order_update.exchange) == 0);
  assert(_symbol.compare(market_by_order_update.symbol) == 0);
  LOG_IF(INFO, _download)(R"(MarketByOrderUpdate={})", market_by_order_update);
  // update depth
  // note!
  //   market by order only gives you *changes*.
  //   you will most likely want to use the the price and order_id
  //   to look up the relative position in an order book and then
  //   modify the liquidity.
  //   the depth builder helps you maintain a correct view of
  //   the order book.
  /*
  _depth_builder->update(market_by_order_update);
  VLOG(1)(
      R"([{}:{}] depth=[{}])",
      _exchange,
      _symbol,
      fmt::join(_depth, ", "));
  validate(_depth);
  */
}

void Instrument::operator()(const OrderUpdate &order_update) {
  // note!
  //   the assumption is that there is never more than one working
  //   order
  if (_last_order_id != order_update.order_id) {
    _last_order_id = order_update.order_id;
    _last_traded_quantity = 0.0;
  }
  auto quantity = order_update.traded_quantity - _last_traded_quantity;
  _last_traded_quantity = order_update.traded_quantity;
  switch (order_update.side) {
    case Side::BUY:
      _long_position += quantity;
      break;
    case Side::SELL:
      _short_position += quantity;
      break;
    default:
      assert(false);  // unexpected
  }
  LOG(INFO)(R"([{}:{}] position={})", _exchange, _symbol, position());
}

void Instrument::operator()(const PositionUpdate &position_update) {
  assert(_account.compare(position_update.account) == 0);
  LOG(INFO)
  (R"([{}:{}] position_update={})", _exchange, _symbol, position_update);
  assert(position_update.position >= -TOLERANCE);
  if (_download) {
    // note!
    //   only update positions when downloading
    //   at run-time we're better off maintaining own positions
    //   since the position feed could be broken or very delayed
    switch (position_update.side) {
      case Side::BUY:
        _long_position = position_update.position;
        break;
      case Side::SELL:
        _short_position = position_update.position;
        break;
      default:
        LOG(WARNING)(R"(Unexpected side={})", position_update.side);
    }
  }
}

void Instrument::check_ready() {
  auto before = _ready;
  _ready = _connection_status == ConnectionStatus::CONNECTED &&
           _download == false && _tick_size > TOLERANCE &&
           _min_trade_vol > TOLERANCE && _multiplier > TOLERANCE &&
           _trading_status == TradingStatus::OPEN &&
           _market_data_status == GatewayStatus::READY &&
           _order_manager_status == GatewayStatus::READY;
  LOG_IF(INFO, _ready != before)
  (R"([{}:{}] ready={})", _exchange, _symbol, _ready);
}

void Instrument::reset() {
  _connection_status = ConnectionStatus::DISCONNECTED;
  _download = false;
  _tick_size = std::numeric_limits<double>::quiet_NaN();
  _min_trade_vol = std::numeric_limits<double>::quiet_NaN();
  _trading_status = TradingStatus::UNDEFINED;
  _market_data_status = GatewayStatus::DISCONNECTED;
  _order_manager_status = GatewayStatus::DISCONNECTED;
  _depth_builder->reset();
  _long_position = 0.0;
  _short_position = 0.0;
  _ready = false;
  _last_order_id = 0;
  _last_traded_quantity = 0.0;
}

void Instrument::validate(const Depth &depth) {
  if (std::fabs(depth[0].bid_quantity) < TOLERANCE ||
      std::fabs(depth[0].ask_quantity) < TOLERANCE)
    return;
  auto spread = depth[0].ask_price - depth[0].bid_price;
  LOG_IF(FATAL, spread < TOLERANCE)
  (R"([{}:{}] Probably something wrong: )"
   R"(choice or inversion detected. depth=[{}])",
   _exchange,
   _symbol,
   fmt::join(depth, ", "));
}

}  // namespace example_3
}  // namespace samples
}  // namespace roq
