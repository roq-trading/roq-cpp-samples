/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "utilities/matcher.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <limits>

namespace examples {
namespace utilities {

Matcher::Matcher(
    roq::simulation::Matcher::Dispatcher& dispatcher,
    const std::string& name)
    : roq::simulation::Matcher(dispatcher, name) {
  if (name.empty())
    throw std::runtime_error("Gateway name is required");
}

void Matcher::on(const roq::BatchBeginEvent& event) {
}

void Matcher::on(const roq::BatchEndEvent& event) {
}

void Matcher::on(const roq::SessionStatisticsEvent& event) {
}

void Matcher::on(const roq::DailyStatisticsEvent& event) {
}

void Matcher::on(const roq::MarketByPriceEvent& event) {
  const roq::MarketByPrice& market_data = event.market_by_price;
  _order_book[std::string(market_data.symbol)] =
                  InstrumentOrderBook(market_data.depth, market_data.depth+roq::MAX_DEPTH);
}

void Matcher::on(const roq::TradeSummaryEvent& event) {
}

void Matcher::on(const roq::CreateOrder& create_order) {
  auto account = create_order.account;
  auto order_id = create_order.order_id;
  switch (create_order.side) {
    case roq::Side::Buy:
    case roq::Side::Sell:
      break;
    default: {
      auto create_order_ack = roq::CreateOrderAck {
        .account = account,
        .order_id = order_id,
        .failure = true,
        .reason = "Invalid side",
        .order_local_id = 0,
        .order_external_id = "",
      };
      send(create_order_ack);
      return;
    }
  }
  auto exchange = create_order.exchange;
  auto symbol = create_order.symbol;
  auto side = create_order.side;
  auto quantity = create_order.quantity;

  auto create_order_ack = roq::CreateOrderAck {
    .account = account,
    .order_id = order_id,
    .failure = false,
    .reason = "",
    .order_local_id = 0,
    .order_external_id = "",
  };
  send(create_order_ack);

  auto order_update = roq::OrderUpdate {
    .account = account,
    .order_id = order_id,
    .exchange = exchange,
    .symbol = symbol,
    .order_status = roq::OrderStatus::Completed,
    .side = side,
    .remaining_quantity = 0.0,
    .traded_quantity = quantity,
    .position_effect = create_order.position_effect,
    .order_template = create_order.order_template,
    .insert_time = std::chrono::time_point_cast<roq::duration_t>(
            std::chrono::system_clock::time_point()),
    .cancel_time = std::chrono::time_point_cast<roq::duration_t>(
            std::chrono::system_clock::time_point()),
    .order_local_id = 0,
    .order_external_id = "",
  };

  auto trade_update = roq::TradeUpdate {
    .account = account,
    .trade_id = order_id,
    .order_id = order_id,
    .exchange = exchange,
    .symbol = symbol,
    .side = side,
    .quantity = 0.0,
    .price = create_order.limit_price,
    .position_effect = create_order.position_effect,
    .order_template = create_order.order_template,
    .trade_time = std::chrono::time_point_cast<roq::duration_t>(
            std::chrono::system_clock::time_point()),
    .order_external_id = "",
    .trade_external_id = "",
  };

  auto& position = _positions[account][exchange][symbol][side];

  auto position_update = roq::PositionUpdate {
      .account = account,
      .exchange = exchange,
      .symbol = symbol,
      .side = side,
      .last_trade_id = order_id,
      .position = position,
      .position_cost = 0.0,
      .position_yesterday = 0.0,
      .position_cost_yesterday = 0.0,
  };

  const InstrumentOrderBook& order_book = _order_book[std::string(create_order.symbol)];
  double price = create_order.limit_price;

  if (create_order.side == roq::Side::Buy) {
    // Check ask side, ask
    for (size_t i = 0; i < order_book.size() && quantity > 0.0; i++) {
      const roq::Layer& layer = order_book[i];
      if (layer.ask_price <= price) {
        trade_update.price = layer.ask_price;
        // Good thing is that we don't need to update order book.
        if (layer.ask_quantity >= quantity) {
          order_update.traded_quantity = quantity;
          order_update.remaining_quantity = 0.0;
          send(order_update);

          trade_update.quantity = quantity;
          send(trade_update);

          position += trade_update.quantity;
          position_update.position = position;
          send(position_update);
          break;
        } else {
          // layer does not have enough quantity
          order_update.traded_quantity = layer.ask_quantity;
          quantity -= layer.ask_quantity;
          order_update.remaining_quantity = quantity;
          send(order_update);

          trade_update.quantity = layer.ask_quantity;
          send(trade_update);

          position += trade_update.quantity;
          position_update.position = position;
          send(position_update);
        }
      } else {
        if (i == 0) {
            // Cancel if layer 0 can not satisfy
            order_update.order_status = roq::OrderStatus::Canceled;
            order_update.traded_quantity = 0.0;
            order_update.remaining_quantity = create_order.quantity;
            send(order_update);
        }
        break;
      }
    }
  } else if (create_order.side == roq::Side::Sell) {
    // Check bid side
    for (size_t i = 0; i < order_book.size() && quantity > 0.0; i++) {
      const roq::Layer& layer = order_book[i];
      if (layer.bid_price >= price) {
        trade_update.price = layer.bid_price;
        if (layer.bid_quantity >= quantity) {
          order_update.traded_quantity = quantity;
          order_update.remaining_quantity = 0.0;
          send(order_update);

          trade_update.quantity = quantity;
          send(trade_update);

          position += trade_update.quantity;
          position_update.position = position;
          send(position_update);
          break;
        } else {
          // layer does not have enough quantity
          order_update.traded_quantity = layer.bid_quantity;
          quantity -= layer.bid_quantity;
          order_update.remaining_quantity = quantity;
          send(order_update);

          trade_update.quantity = layer.bid_quantity;
          send(trade_update);

          position += trade_update.quantity;
          position_update.position = position;
          send(position_update);
        }
      } else {
        if (i == 0) {
            order_update.order_status = roq::OrderStatus::Canceled;
            order_update.traded_quantity = 0.0;
            order_update.remaining_quantity = create_order.quantity;
            send(order_update);
        }
        break;
      }
    }
  }
}

void Matcher::on(const roq::ModifyOrder& modify_order) {
  auto modify_order_ack = roq::ModifyOrderAck {
    .account = modify_order.account,
    .order_id = modify_order.order_id,
    .failure = true,
    .reason = "NOT_SUPPORTED",
    .order_local_id = 0,
    .order_external_id = "",
  };
  send(modify_order_ack);
}

void Matcher::on(const roq::CancelOrder& cancel_order) {
  auto cancel_order_ack = roq::CancelOrderAck {
    .account = cancel_order.account,
    .order_id = cancel_order.order_id,
    .failure = true,
    .reason = "NOT_SUPPORTED",
    .order_local_id = 0,
    .order_external_id = "",
  };
  send(cancel_order_ack);
}

}  // namespace utilities
}  // namespace examples
