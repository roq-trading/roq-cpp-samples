/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/algo-proto/bridge.hpp"

#include <algorithm>
#include <memory>
#include <utility>

#include "roq/client.hpp"
#include "roq/logging.hpp"

#include "roq/samples/algo-proto/flags/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace algo_proto {

// === HELPERS ===

namespace {
template <typename R>
auto create_routes(auto const &config) {
  R result;
  uint32_t market_id = 0;
  for (auto &[exchange, symbols] : config.exchange_symbols) {
    auto &tmp = result[exchange];
    for (auto &symbol : symbols) {
      [[maybe_unused]] auto res = tmp.try_emplace(symbol, ++market_id, exchange, symbol);
      assert(res.second);
    }
  }
  return result;
}

template <typename Callback>
bool find_route(auto &routes, std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
  auto iter_1 = routes.find(exchange);
  if (iter_1 == std::end(routes))
    return false;
  auto &tmp = (*iter_1).second;
  auto iter_2 = tmp.find(symbol);
  if (iter_2 == std::end(tmp))
    return false;
  auto &route = (*iter_2).second;
  callback(route);
  return true;
}

template <typename R>
auto create_factories(auto const &config, auto const &routes, auto const &gateways) {
  R result;
  for (auto &[symbol, strategy] : config.strategies) {
    std::vector<algo::framework::Market const *> tmp;
    tmp.reserve(std::size(strategy.instruments));
    for (auto &instrument : strategy.instruments) {
      if (find_route(routes, instrument.exchange, instrument.symbol, [&](auto &route) {
            auto &market = static_cast<const algo::framework::Market &>(route);
            tmp.emplace_back(&market);
          })) {
      } else {
        log::fatal("Unexpected"sv);
      }
    }
    // XXX should be external
    auto factory = std::make_unique<algo::strategies::Factory>(strategy.type, strategy.instruments, tmp, gateways);
    result.try_emplace(symbol, std::move(factory));
  }
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

Bridge::Bridge(client::Dispatcher &dispatcher, Config const &config, size_t size)
    : dispatcher_{dispatcher}, routes_{create_routes<decltype(routes_)>(config)},
      gateways_(size), factories_{create_factories<decltype(factories_)>(config, routes_, gateways_)} {
  auto side = magic_enum::enum_cast<Side>(flags::Flags::side()).value_or(Side::UNDEFINED);
  CreateOrder create_order{
      .account = flags::Flags::account(),
      .order_id = {},
      .exchange = {},
      .symbol = flags::Flags::strategy(),
      .side = side,
      .position_effect = {},
      .max_show_quantity = NaN,
      .order_type = OrderType::LIMIT,
      .time_in_force = TimeInForce::GTC,
      .execution_instructions = {},
      .order_template{},
      .quantity = flags::Flags::quantity(),
      .price = flags::Flags::price(),
      .stop_price = NaN,
      .routing_id = {},
  };
  create_strategy(create_order);  // XXX must wait until all sources connected
}

// algo::framework::Dispatcher

void Bridge::operator()(CreateOrder const &create_order) {
  assert(!std::empty(create_order.routing_id));
  max_order_id_ = std::max(max_order_id_, create_order.order_id);
  if (find_route(routes_, create_order.exchange, create_order.symbol, [&](auto &route) {
        auto source = route.get_source();
        log::debug("create_order={}, source={}"sv, create_order, source);
        order_id_to_source_.emplace(create_order.order_id, source);
        dispatcher_.send(create_order, source);
      })) {
  } else {
    log::fatal(R"(Unknown exchange="{}", symbol="{}")"sv, create_order.exchange, create_order.symbol);
  }
}

void Bridge::operator()(ModifyOrder const &modify_order) {
  assert(!std::empty(modify_order.routing_id));
  max_order_id_ = std::max(max_order_id_, modify_order.order_id);
  if (find_source(modify_order.order_id, [&](auto source) { dispatcher_.send(modify_order, source); })) {
  } else {
    log::fatal("Unexpected"sv);
  }
}

void Bridge::operator()(CancelOrder const &cancel_order) {
  assert(!std::empty(cancel_order.routing_id));
  max_order_id_ = std::max(max_order_id_, cancel_order.order_id);
  if (find_source(cancel_order.order_id, [&](auto source) { dispatcher_.send(cancel_order, source); })) {
  } else {
    log::fatal("Unexpected"sv);
  }
}

// client::Handler

void Bridge::operator()(Event<Timer> const &event) {
  for (auto &[_, strategy] : strategies_)
    (*strategy)(event);
}

void Bridge::operator()(Event<Connected> const &event) {
  auto &[message_info, connected] = event;
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<Disconnected> const &event) {
  auto &[message_info, disconnected] = event;
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<DownloadBegin> const &event) {
  auto &[message_info, download_begin] = event;
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<DownloadEnd> const &event) {
  auto &[message_info, download_end] = event;
  max_order_id_ = std::max(max_order_id_, download_end.max_order_id);
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<GatewaySettings> const &event) {
  auto &[message_info, gateway_settings] = event;
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<GatewayStatus> const &event) {
  auto &[message_info, gateway_status] = event;
  if (gateways_[message_info.source](event)) {
    for (auto &[_, strategy] : strategies_)
      (*strategy)(event);
  }
}

void Bridge::operator()(Event<ReferenceData> const &event) {
  dispatch(event);
}

void Bridge::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Bridge::operator()(Event<TopOfBook> const &event) {
  dispatch(event);
}

void Bridge::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event);
}

void Bridge::operator()(Event<OrderAck> const &event) {
  auto &[message_info, order_ack] = event;
  max_order_id_ = std::max(max_order_id_, order_ack.order_id);
  if (find_strategy(order_ack.routing_id, [&](auto &strategy) { strategy(event); })) {
  } else {
    log::fatal(R"(Unexpected: routing_id="{}")"sv, order_ack.routing_id);
  }
}

void Bridge::operator()(Event<OrderUpdate> const &event) {
  auto &[message_info, order_update] = event;
  max_order_id_ = std::max(max_order_id_, order_update.order_id);
  // XXX accumulate positions by strategy
  if (find_strategy(order_update.routing_id, [&](auto &strategy) { strategy(event); })) {
  } else {
    log::fatal(R"(Unexpected: routing_id="{}")"sv, order_update.routing_id);
  }
}

void Bridge::operator()(Event<TradeUpdate> const &) {
  // XXX maybe useful for correlating positions?
}

void Bridge::operator()(Event<PositionUpdate> const &) {
  // XXX maybe useful for correlating positions?
}

template <typename T>
void Bridge::dispatch(Event<T> const &event) {
  auto &[message_info, value] = event;
  if (find_route(routes_, value.exchange, value.symbol, [&](auto &route) {
        route.dispatch(event, [&](auto strategy_id) {
          auto iter = strategies_.find(strategy_id);
          if (iter != std::end(strategies_)) {
            auto &order = *(*iter).second;
            order(event);
          } else {
            assert(false);
          }
        });
      })) {
  } else {
    assert(false);
  }
}

void Bridge::create_strategy(CreateOrder const &create_order) {
  auto iter = factories_.find(create_order.symbol);
  if (iter != std::end(factories_)) {
    auto &factory = *(*iter).second;
    auto strategy_id = ++strategy_id_;
    auto routing_id =
        client::Base64::encode({reinterpret_cast<std::byte const *>(&strategy_id), sizeof(strategy_id)}, true);
    auto strategy = factory.create(*this, routing_id, create_order);
    assert(!!strategy);
    [[maybe_unused]] auto res = strategies_.emplace(strategy_id, std::move(strategy));
    assert(res.second);
    auto &state = static_cast<algo::framework::State const &>(factory);
    state.dispatch([&](auto &instrument) {
      if (find_route(routes_, instrument.exchange, instrument.symbol, [&](auto &route) { route.add(strategy_id); })) {
      } else {
        assert(false);
      }
    });
    routing_id_to_strategy_.emplace(routing_id, strategy_id);
  } else {
    log::fatal(R"(Invalid symbol="{}")"sv, create_order.symbol);
  }
}

template <typename Callback>
bool Bridge::find_strategy(std::string_view const &routing_id, Callback callback) {
  auto iter_1 = routing_id_to_strategy_.find(routing_id);
  if (iter_1 == std::end(routing_id_to_strategy_))
    return false;
  auto iter_2 = strategies_.find((*iter_1).second);
  if (iter_2 == std::end(strategies_))
    return false;
  auto &strategy = *(*iter_2).second;
  callback(strategy);
  return true;
}

template <typename Callback>
bool Bridge::find_source(uint32_t order_id, Callback callback) {
  auto iter = order_id_to_source_.find(order_id);
  if (iter == std::end(order_id_to_source_))
    return false;
  callback((*iter).second);
  return true;
}

}  // namespace algo_proto
}  // namespace samples
}  // namespace roq
