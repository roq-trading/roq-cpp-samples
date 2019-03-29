/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/simple/strategy.h"

#include "roq/logging.h"
#include "roq/stream.h"

namespace roq {
namespace samples {
namespace simple {

Strategy::Strategy(
    client::Dispatcher& dispatcher,
    const std::string& exchange,
    const std::vector<std::string_view>& accounts,
    const std::vector<std::string_view>& symbols,
    bool create_orders,
    std::chrono::milliseconds create_order_timeout)
    : _dispatcher(dispatcher),
      _exchange(exchange),
      _trade_account(*accounts.begin()),
      _trade_symbol(*symbols.begin()),
      _create_orders(create_orders),
      _create_order_timeout(create_order_timeout),
      _market_by_price(
          client::DepthBuilder::create(_depth, std::size(_depth))) {
}

void Strategy::on(const StartEvent& event) {
  // This is a "hook" allowing you to safely start other threads.
  //
  // A "back-door" which isn'really compatible with simulation.
}

void Strategy::on(const StopEvent& event) {
  // This is a "hook" allowing you to safely stop other threads.
  //
  // A "back-door" which isn't compatible with simulation.
}

void Strategy::on(const TimerEvent& event) {
  // Important!
  // You should *never* access the system clock.
  // Instead, use the timer event to learn about current time.
  // Follow this rule so that you can run your strategy through
  // a simulator.
  //
  // A typical pattern is to check for order creation timeout.
  if (_order_timeout.count() == 0 || event.now < _order_timeout)
    return;
  LOG(WARNING) << "Timeout -- never received ack for order_id=" << _order_id;
  _order_timeout = {};
}

void Strategy::on(const ConnectionStatusEvent& event) {
  // This event will be generated when the connection status changes.
  // You will receive the first event when a connection is successfully
  // established. Following events will happen when a disconnect has
  // been detected (by missed heartbeat), then the next successful
  // connection, etc.
  auto source = event.source_name;
  switch (event.connection_status) {
    case ConnectionStatus::CONNECTED: {
      LOG(INFO) << "Connected to source=\"" << source << "\"";
      LOG(INFO) << "Subscribe account=\"" << _trade_account << "\", "
        "symbol=\"" << _trade_symbol << "\"";
      // Here we must subscribe to accounts and symbols (per exchange)
      _dispatcher.send(
          Subscribe {
            .accounts = { _trade_account },
            .symbols_by_exchange = { {
                _exchange.c_str(), { _trade_symbol }
              } }
          },
          event.source);
      break;
    }
    case ConnectionStatus::DISCONNECTED: {
      LOG(INFO) << "Disconnected from source=\"" << source << "\"";
      _order_manager_ready = false;
      _market_data_ready = false;
      _market_by_price->clear();
      _market_open = false;
      break;
    }
  }
}

void Strategy::on(const BatchBeginEvent& event) {
  // This event is always dispatched to mark the beginning of a
  // sequence of batched events.
}

void Strategy::on(const BatchEndEvent& event) {
  // This event marks the end of a sequence of batched events.
  //
  // Not required, but a typical pattern is to defer processing until
  // a consistent view has been received fully.
  if (_dirty) {
    _dirty = false;
    process_update(event.message_info.receive_time);
  }
}

void Strategy::on(const MarketDataStatusEvent& event) {
  // This event is generated when the gateway has recorded
  // a changed connection status for the market data feed.
  //
  // This event can be received both during and after download.
  switch (event.market_data_status.status) {
    case GatewayStatus::READY: {
      if (!_market_data_ready) {
        LOG(INFO) << "Market data is ready";
        _market_data_ready = true;
      }
      break;
    }
    default: {
      if (_market_data_ready) {
        LOG(INFO) << "Market data is *not* ready";
        _market_data_ready = false;
        _market_by_price->clear();
        _market_open = false;
      }
    }
  }
}

void Strategy::on(const OrderManagerStatusEvent& event) {
  // This event is generated when the gateway has recorded
  // a changed connection status for an order management connection.
  //
  // This event can be received both during and after download.
  auto account = event.order_manager_status.account;
  if (_trade_account.compare(account) != 0)
    return;
  switch (event.order_manager_status.status) {
    case GatewayStatus::READY: {
      if (!_order_manager_ready) {
        _order_manager_ready = true;
        LOG(INFO) << "Order manager is ready for "
          "account=\"" << account << "\"";
      }
      break;
    }
    default: {
      if (_order_manager_ready) {
        _order_manager_ready = false;
        LOG(INFO) << "Order manager is *not* ready for "
          "account=\"" << account << "\"";
      }
    }
  }
}

void Strategy::on(const DownloadBeginEvent& event) {
  // This event marks the start of the download phase.
  //
  // Important!
  // You are *not* allowed to send *any* kind of request to the
  // dispatcher interface until *either* the download phase has
  // completed *or* a disconnect has been detected.
  auto account = event.download_begin.account;
  if (std::strlen(account) == 0) {
    LOG(INFO) << "Downloading market data ...";
  } else if (_trade_account.compare(account) == 0) {
    LOG(INFO) << "Downloading order manager data for "
      "account=\"" << account << "\" ...";
  } else {
    LOG(FATAL) << "Unexpected -- account=\"" << account << "\"";
  }
}

void Strategy::on(const DownloadEndEvent& event) {
  // This event marks the end of the download phase.
  //
  // Note!
  // You are again allowed to send requests to the dispatcher
  // interface.
  auto account = event.download_end.account;
  if (std::strlen(account) == 0) {
    LOG(INFO) << "Market data download has completed";
  } else if (_trade_account.compare(account) == 0) {
    // Local order id's are required to *never* repeat.
    // We can manage ensure this by always increasing the local order
    // id's *and* by starting from the maximum order id already known
    // by the order manager.
    auto order_id = std::max(
        _order_id,
        event.download_end.max_order_id);
    if (order_id != _order_id) {
      _order_id = order_id;
      LOG(INFO) << "Update: order_id=" << _order_id;
    }
    LOG(INFO) << "Order manager data download has completed for "
      "account=\"" << account << "\"";
  } else {
    LOG(FATAL) << "Unexpected -- account=\"" << account << "\"";
  }
}

void Strategy::on(const ReferenceDataEvent& event) {
  // Reference data for a symbol, e.g. tick-size.
}

void Strategy::on(const MarketStatusEvent& event) {
  // Market status for a symbol, e.g. open for trading.
  switch (event.market_status.trading_status) {
    case TradingStatus::OPEN: {
      if (!_market_open) {
        _market_open = true;
        LOG(INFO) << "Market is open for trading";
      }
      break;
    }
    default: {
      LOG(INFO) << "Market is *not* open for trading";
      _market_open = false;
    }
  }
}

void Strategy::on(const SessionStatisticsEvent& event) {
  // Session statistics for a symbol, e.g. highest/lowest price.
}

void Strategy::on(const DailyStatisticsEvent& event) {
  // Daily statistics for a symbol, e.g. open interest.
}

void Strategy::on(const MarketByPriceEvent& event) {
  // Market by price update for a symbol.
  _market_by_price->update(event.market_by_price, true);
  _dirty = true;
}

void Strategy::on(const TradeSummaryEvent& event) {
  // Trade summary for a symbol, e.g. last traded price and quantity.
}

void Strategy::on(const PositionUpdateEvent& event) {
  // Position update.
  //
  // Important!
  // Positions are managed by account, symbol and side.
  // There *may* be additional information, e.g. yesterday's
  // close position, locked position, etc.
  //
  // Note!
  // Positions can be received by *all* strategies.
}

void Strategy::on(const OrderUpdateEvent& event) {
  // Order update.
  //
  // Important!
  // You will only see order updates for those orders
  // created by this strategy.
  // It is therefore possible for multiple strategies to
  // safely trade on the same account(s).
  auto order_id = event.order_update.order_id;
  // Following checks will only issue warning since the cause could
  // be external (API's or market, e.g. old messages being repeated)
  // ... different order_id indicates out-of-sync updates
  LOG_IF(WARNING, order_id != _order_id) << "Unexpected";
  // ... timeout can only be reset by ack indicating failure
  LOG_IF(WARNING, _order_timeout.count() == 0) << "Unexpected";
  // reset the timeout
  if (order_id == _order_id && _order_timeout.count() != 0) {
    _order_timeout = {};
  }
}

void Strategy::on(const TradeUpdateEvent& event) {
  // Trade update.
  //
  // Important!
  // You will only see trade updates originating from orders
  // created by this strategy.
  // It is therefore possible for multiple strategies to
  // safely trade on the same account(s).
}

void Strategy::on(const CreateOrderAckEvent& event) {
  // Reponse to a CreateOrder request.
  //
  // Note!
  // May originate locally, from gateway or from broker.
  auto order_id = event.create_order_ack.order_id;
  // Following checks will terminate processing since our internal state
  // management will be incorrect if we continue
  // ... different order_id indicates out-of-sync updates (there should
  // only be one in-flight order creation request)
  LOG_IF(FATAL, order_id != _order_id) << "Unexpected";
  // ... timeout can only be reset by ack indicating failure
  LOG_IF(FATAL, _order_timeout.count() == 0) << "Unexpected";
  if (event.create_order_ack.failure) {
    _order_timeout = {};
  } else {
    // otherwise wait for the order update
  }
}

void Strategy::on(const ModifyOrderAckEvent& event) {
  // Reponse to a ModifyOrder request.
  //
  // Note!
  // May originate locally, from gateway or from broker.
  LOG(FATAL) << "Unexpected";
}

void Strategy::on(const CancelOrderAckEvent& event) {
  // Reponse to a CancelOrder request.
  //
  // Note!
  // May originate locally, from gateway or from broker.
  LOG(FATAL) << "Unexpected";
}

void Strategy::on(const CustomMessageEvent& event) {
  // This is a back-door allowing you to integrate, e.g.
  // third-party data feeds.
  LOG(FATAL) << "Unexpected";
}

void Strategy::write_metrics(std::ostream& stream) {
  // Handler allowing you to export internal metrics.
  //
  // The application can optionally expose an end-point allowing
  // a monitoring system, e.g. Prometheus, to capture the time-series
  // of metrics.
  //
  // Important!
  // This method is *not* called from the main dispatch thread.
  // Thus, make sure to not access non-const variables from here.
  // You should preferably only use atomic variables for collecting
  // metrics.
  //
  // Format:
  // https://github.com/prometheus/docs/blob/master/content/docs/instrumenting/exposition_formats.md
}

void Strategy::process_update(std::chrono::nanoseconds now) {
  VLOG(1) << "bid=" << _depth[0].bid_price << ", "
    "ask=" << _depth[0].ask_price;
  // order creation must be explicitly requested
  if (!_create_orders)
    return;
  LOG_IF(WARNING, !_order_manager_ready) <<
    "Not connected to order manager!";
  LOG_IF(WARNING, !_market_open) <<
    "Market may not be open for trading";
  try_trade(
      Side::BUY,
      _depth[0].ask_price,
      1.0,
      now);
}

void Strategy::try_trade(
    Side side,
    double price,
    double quantity,
    std::chrono::nanoseconds now) {
  if (_order_timeout.count()) {
    VLOG(1) << "Refusing to trade -- another order is already in-flight";
    return;
  }
  auto order_id = ++_order_id;
  CreateOrder create_order {
    .account = _trade_account.c_str(),
    .order_id = order_id,
    .exchange = _exchange.c_str(),
    .symbol = _trade_symbol.c_str(),
    .side = side,
    .quantity = quantity,
    .order_type = OrderType::LIMIT,
    .limit_price = price,
    .time_in_force = TimeInForce::GTC,
    .position_effect = PositionEffect::UNDEFINED,
    .order_template = "FOK"
  };
  try {
    auto source = 0;
    _dispatcher.send(create_order, source);
    _order_timeout = now + _create_order_timeout;
  } catch (NotReady&) {
    LOG(WARNING) << "Order manager is *not* in a ready state";
  } catch (NotConnected&) {
    LOG(FATAL) << "Order manager is *not* connected";
  } catch (...) {
    LOG(FATAL) << "Unexpected";
  }
}

}  // namespace simple
}  // namespace samples
}  // namespace roq
