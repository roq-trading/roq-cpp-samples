/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <cassert>

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>

#include "roq/application.h"
#include "roq/client.h"
#include "roq/event.h"
#include "roq/logging.h"

// command-line options

DEFINE_string(exchange,
    "deribit",
    "exchange");

DEFINE_string(symbol,
    "BTC-27MAR20",
    "symbol (regex)");

DEFINE_string(account,
    "A1",
    "account");

DEFINE_string(currencies,
    "BTC|USD",
    "currencies (regex)");

DEFINE_uint32(sample_freq_secs,
    uint32_t{1},
    "sample frequency (seconds)");

DEFINE_double(ema_alpha,
    double{0.33},
    "alpha used to compute exponential moving average (0 < alpha <= 1)");
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

DEFINE_uint32(warmup,
    uint32_t{120},
    "warmup (number of samples before a signal is generated)");

DEFINE_bool(enable_trading,
    false,
    "trading must explicitly be enabled!");

DEFINE_bool(simulation,
    false,
    "requires an event-log");

namespace roq {
namespace samples {
namespace example_3 {

// constants

namespace {
constexpr auto NaN = std::numeric_limits<double>::quiet_NaN();
constexpr auto TOLERANCE = double{1.0e-10};
constexpr auto MAX_DEPTH = size_t{3};
}  // namespace

// convenience

using Depth = std::array<Layer, MAX_DEPTH>;

// utilities

namespace {
template <typename T>
inline bool update(T& lhs, const T& rhs) {
  if (lhs == rhs)  // note! too simplistic for T == double
    return false;
  lhs = rhs;
  return true;
}
}  // namespace

// configuration

class Config final : public client::Config {
 public:
  Config() {}

  Config(const Config&) = delete;
  Config(Config&&) = default;

 protected:
  void dispatch(Handler& handler) const override {
    // accounts
    handler(
        client::Account {
          .regex = FLAGS_account,
        });
    // symbols
    handler(
        client::Symbol {
          .regex = FLAGS_symbol,
          .exchange = FLAGS_exchange,
        });
    // currencies
    handler(
        client::Symbol {
          .regex = FLAGS_currencies,
        });
  }
};

// helper class caching instrument specific information

class Instrument final {
 public:
  Instrument(
      const std::string_view& exchange,
      const std::string_view& symbol,
      const std::string_view& account)
      : _exchange(exchange),
        _symbol(symbol),
        _account(account),
        _depth_builder(
            client::DepthBuilder::create(
                symbol,
                _depth)) {
  }

  operator const Depth&() const {
    return _depth;
  }

  bool is_ready() const {
    return _ready;
  }

  auto tick_size() const {
    return _tick_size;
  }

  auto min_trade_vol() const {
    return _min_trade_vol;
  }

  auto multiplier() const {
    return _multiplier;
  }

  auto is_market_open() const {
    return _trading_status == TradingStatus::OPEN;
  }

  auto best_bid() const {
    return _depth[0].bid_price;
  }

  auto best_ask() const {
    return _depth[0].ask_price;
  }

  auto position() const {
    return (std::isnan(_long_position) ? double{0.0} : _long_position)
      - (std::isnan(_short_position) ? double{0.0} : _short_position);
  }

  auto can_trade(Side side) const {
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

  void operator()(const ConnectionStatus& connection_status) {
    if (update(_connection_status, connection_status)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] connection_status={}"),
          _exchange,
          _symbol,
          _connection_status);
      check_ready();
    }
    switch (connection_status) {
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

  void operator()(const DownloadBegin& download_begin) {
    if (download_begin.account.empty() == false)
      return;
    assert(_download == false);
    _download = true;
    LOG(INFO)(
        FMT_STRING("[{}:{}] download={}"),
          _exchange,
          _symbol,
        _download);
  }

  void operator()(const DownloadEnd& download_end) {
    if (download_end.account.empty() == false)
      return;
    assert(_download == true);
    _download = false;
    LOG(INFO)(
        FMT_STRING("[{}:{}] download={}"),
          _exchange,
          _symbol,
        _download);
    // update the ready flag
    check_ready();
  }

  void operator()(const MarketDataStatus& market_data_status) {
    // update our cache
    if (update(_market_data_status, market_data_status.status)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] market_data_status={}"),
          _exchange,
          _symbol,
          _market_data_status);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const OrderManagerStatus& order_manager_status) {
    assert(_account.compare(order_manager_status.account) == 0);
    // update our cache
    if (update(_order_manager_status, order_manager_status.status)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] order_manager_status={}"),
          _exchange,
          _symbol,
          _order_manager_status);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const ReferenceData& reference_data) {
    assert(_exchange.compare(reference_data.exchange) == 0);
    assert(_symbol.compare(reference_data.symbol) == 0);
    // update the depth builder
    _depth_builder->update(reference_data);
    // update our cache
    if (update(_tick_size, reference_data.tick_size)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] tick_size={}"),
          _exchange,
          _symbol,
          _tick_size);
    }
    if (update(_min_trade_vol, reference_data.min_trade_vol)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] min_trade_vol={}"),
          _exchange,
          _symbol,
          _min_trade_vol);
    }
    if (update(_multiplier, reference_data.multiplier)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] multiplier={}"),
          _exchange,
          _symbol,
          _multiplier);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const MarketStatus& market_status) {
    assert(_exchange.compare(market_status.exchange) == 0);
    assert(_symbol.compare(market_status.symbol) == 0);
    // update our cache
    if (update(_trading_status, market_status.trading_status)) {
      LOG(INFO)(
          FMT_STRING("[{}:{}] trading_status={}"),
          _exchange,
          _symbol,
          _trading_status);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const MarketByPrice& market_by_price) {
    assert(_exchange.compare(market_by_price.exchange) == 0);
    assert(_symbol.compare(market_by_price.symbol) == 0);
    LOG_IF(INFO, _download)(
        FMT_STRING("MarketByPrice={}"),
        market_by_price);
    // update depth
    // note!
    //   market by price only gives you *changes*.
    //   you will most likely want to use the the price to look up
    //   the relative position in an order book and then modify the
    //   liquidity.
    //   the depth builder helps you maintain a correct view of
    //   the order book.
    _depth_builder->update(market_by_price);
    VLOG(1)(
        FMT_STRING("[{}:{}] depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    validate(_depth);
  }

  void operator()(const MarketByOrder& market_by_order) {
    assert(_exchange.compare(market_by_order.exchange) == 0);
    assert(_symbol.compare(market_by_order.symbol) == 0);
    LOG_IF(INFO, _download)(
        FMT_STRING("MarketByOrder={}"),
        market_by_order);
    // update depth
    // note!
    //   market by order only gives you *changes*.
    //   you will most likely want to use the the price and order_id
    //   to look up the relative position in an order book and then
    //   modify the liquidity.
    //   the depth builder helps you maintain a correct view of
    //   the order book.
    /*
    _depth_builder->update(market_by_order);
    VLOG(1)(
        FMT_STRING("[{}:{}] depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    validate(_depth);
    */
  }

  void operator()(const OrderUpdate& order_update) {
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
    LOG(INFO)(
        FMT_STRING("[{}:{}] position={}"),
        _exchange,
        _symbol,
        position());
  }

  void operator()(const PositionUpdate& position_update) {
    assert(_account.compare(position_update.account) == 0);
    LOG(INFO)(
        FMT_STRING("[{}:{}] position_update={}"),
        _exchange,
        _symbol,
        position_update);
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
          LOG(WARNING)(
              FMT_STRING("Unexpected side={}"),
              position_update.side);
      }
    }
  }

 protected:
  void check_ready() {
    auto before = _ready;
    _ready =
      _connection_status == ConnectionStatus::CONNECTED &&
      _download == false &&
      _tick_size > TOLERANCE &&
      _min_trade_vol > TOLERANCE &&
      _multiplier > TOLERANCE &&
      _trading_status == TradingStatus::OPEN &&
      _market_data_status == GatewayStatus::READY &&
      _order_manager_status == GatewayStatus::READY;
    LOG_IF(INFO, _ready != before)(
        FMT_STRING("[{}:{}] ready={}"),
        _exchange,
        _symbol,
        _ready);
  }

  void reset() {
    _connection_status = ConnectionStatus::DISCONNECTED;
    _download = false;
    _tick_size = NaN;
    _min_trade_vol = NaN;
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

  void validate(const Depth& depth) {
    if (std::fabs(depth[0].bid_quantity) < TOLERANCE ||
        std::fabs(depth[0].ask_quantity) < TOLERANCE)
      return;
    auto spread = depth[0].ask_price - depth[0].bid_price;
    LOG_IF(FATAL, spread < TOLERANCE)(
        FMT_STRING(
          "[{}:{}] Probably something wrong: "
          "choice or inversion detected. depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(depth, ", "));
  }

 private:
  const std::string_view _exchange;
  const std::string_view _symbol;
  const std::string_view _account;
  ConnectionStatus _connection_status = ConnectionStatus::DISCONNECTED;
  bool _download = false;
  double _tick_size = NaN;
  double _min_trade_vol = NaN;
  double _multiplier = NaN;
  TradingStatus _trading_status = TradingStatus::UNDEFINED;
  GatewayStatus _market_data_status = GatewayStatus::DISCONNECTED;
  GatewayStatus _order_manager_status = GatewayStatus::DISCONNECTED;
  Depth _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  double _long_position = 0.0;
  double _short_position = 0.0;
  bool _ready = false;
  uint32_t _last_order_id = 0;
  double _last_traded_quantity = 0.0;
};

// helper

class EMA final {
 public:
  EMA(double alpha)
      : _alpha(alpha) {
  }

  EMA(const EMA&) = delete;
  EMA(EMA&&) = default;

  operator double() const {
    return _value;
  }

  void reset() {
    _value = NaN;
    _countdown = FLAGS_warmup;
  }

  bool is_ready() const {
    return _countdown == 0;
  }

  double update(double value) {
    _countdown = std::max<uint32_t>(1, _countdown) - uint32_t{1};
    if (std::isnan(_value))
      _value = value;
    else
      _value = _alpha * value + (1.0 - _alpha) * _value;
    return _value;
  }

 private:
  const double _alpha;
  double _value = NaN;
  uint32_t _countdown = FLAGS_warmup;
};

// model implementation

class Model final {
 public:
  Model()
      : _bid_ema(FLAGS_ema_alpha),
        _ask_ema(FLAGS_ema_alpha) {
  }

  Model(const Model&) = delete;
  Model(Model&&) = default;

  void reset() {
    _bid_ema.reset();
    _ask_ema.reset();
    _selling = false;
    _buying = false;
  }

  Side update(const Depth& depth) {
    auto result = Side::UNDEFINED;

    if (validate(depth) == false)
      return result;

    auto bid_fast = weighted_bid(depth);
    auto bid_slow = _bid_ema.update(bid_fast);
    auto ask_fast = weighted_ask(depth);
    auto ask_slow = _ask_ema.update(ask_fast);

    auto ready = _bid_ema.is_ready() && _ask_ema.is_ready();

    if (_selling) {
      if (ready && ask_fast > ask_slow) {
        LOG(INFO)(
            FMT_STRING("SIGNAL: BUY @ {}"),
            depth[0].ask_price);
        result = Side::BUY;
        _selling = false;
      }
    } else {
      if (ask_fast < bid_slow && ask_fast < ask_slow) {
        LOG(INFO)("DIRECTION: SELLING");
        _selling = true;
        _buying = false;
      }
    }

    if (_buying) {
      if (ready && bid_fast > bid_slow) {
        LOG(INFO)(
            FMT_STRING("SIGNAL: SELL @ {}"),
            depth[0].bid_price);
        result = Side::SELL;
        _buying = false;
      }
    } else {
      if (bid_fast > ask_slow && bid_fast > bid_slow) {
        LOG(INFO)("DIRECTION: BUYING");
        _buying = true;
        _selling = false;
      }
    }

    // can't be both
    assert(2 != ((_selling ? 1 : 0) + (_buying ? 1 : 0)));

    VLOG(1)(
        FMT_STRING(
          "model={{"
          "bid={} "
          "ask={} "
          "bid_fast={} "
          "ask_fast={} "
          "bid_slow={} "
          "ask_slow={} "
          "selling={} "
          "buying={}"
          "}}"),
        depth[0].bid_price,
        depth[0].ask_price,
        bid_fast,
        ask_fast,
        bid_slow,
        ask_slow,
        _selling,
        _buying);

    return result;
  }

 protected:
  bool validate(const Depth& depth) {  // require full depth
    return std::accumulate(depth.begin(), depth.end(), true,
        [](bool current, const Layer& layer) {
          return current &&
            std::fabs(layer.bid_quantity) > TOLERANCE &&
            std::fabs(layer.ask_quantity) > TOLERANCE;
        });
  }

  double weighted_bid(const Depth& depth) {
    double sum_1 = 0.0, sum_2 = 0.0;
    std::for_each(depth.begin(), depth.end(),
        [&](const Layer& layer) {
          sum_1 += layer.bid_quantity * layer.bid_price;
          sum_2 += layer.bid_quantity;
        });
    return sum_1 / sum_2;
  }

  double weighted_ask(const Depth& depth) {
    double sum_1 = 0.0, sum_2 = 0.0;
    std::for_each(depth.begin(), depth.end(),
        [&](const Layer& layer) {
          sum_1 += layer.ask_quantity * layer.ask_price;
          sum_2 += layer.ask_quantity;
        });
    return sum_1 / sum_2;
  }

 private:
  EMA _bid_ema;
  EMA _ask_ema;
  bool _selling = false;
  bool _buying = false;
};

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher),
        _instrument(
            FLAGS_exchange,
            FLAGS_symbol,
            FLAGS_account) {
  }

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

 protected:
  void operator()(const TimerEvent& event) override {
    // note! using system clock (*not* the wall clock)
    if (event.now < _next_sample)
      return;
    if (_next_sample.count())  // initialized?
      update_model();
    auto now = std::chrono::duration_cast<std::chrono::seconds>(
        event.now);
    _next_sample = now + std::chrono::seconds {
      FLAGS_sample_freq_secs
    };
    // possible extension: reset request timeout
  }
  void operator()(const ConnectionStatusEvent& event) override {
    dispatch(event);
  }
  void operator()(const DownloadBeginEvent& event) override {
    dispatch(event);
  }
  void operator()(const DownloadEndEvent& event) override {
    dispatch(event);
    if (update(_max_order_id, event.download_end.max_order_id)) {
      LOG(INFO)(
          FMT_STRING("max_order_id={}"),
          _max_order_id);
    }
  }
  void operator()(const MarketDataStatusEvent& event) override {
    dispatch(event);
  }
  void operator()(const OrderManagerStatusEvent& event) override {
    dispatch(event);
  }
  void operator()(const ReferenceDataEvent& event) override {
    dispatch(event);
  }
  void operator()(const MarketStatusEvent& event) override {
    dispatch(event);
  }
  void operator()(const MarketByPriceEvent& event) override {
    dispatch(event);
  }
  void operator()(const OrderAckEvent& event) override {
    LOG(INFO)(
        FMT_STRING("OrderAck={}"),
        event_value(event));
    auto& order_ack = event.order_ack;
    if (is_complete(order_ack.status)) {
      // possible extension: reset request timeout
    }
  }
  void operator()(const OrderUpdateEvent& event) override {
    LOG(INFO)(
        FMT_STRING("OrderUpdate={}"),
        event_value(event));
    dispatch(event);  // update position
    auto& order_update = event.order_update;
    if (is_complete(order_update.status)) {
      _working_order_id = 0;
      _working_side = Side::UNDEFINED;
    }
  }
  void operator()(const TradeUpdateEvent& event) override {
    LOG(INFO)(
        FMT_STRING("TradeUpdate={}"),
        event_value(event));
  }
  void operator()(const PositionUpdateEvent& event) override {
    dispatch(event);
  }
  void operator()(const FundsUpdateEvent& event) override {
    LOG(INFO)(
        FMT_STRING("FundsUpdate={}"),
        event_value(event));
  }

  // helper - dispatch event to instrument
  template <typename T>
  void dispatch(const T& event) {
    assert(event.message_info.source == uint8_t{0});
    _instrument(event_value(event));
  }

  void update_model() {
    if (_instrument.is_ready()) {
      auto side = _model.update(_instrument);
      switch (side) {
        case Side::UNDEFINED:
          // nothing to do
          break;
        case Side::BUY:
          try_trade(side, _instrument.best_bid());
          break;
        case Side::SELL:
          try_trade(side, _instrument.best_ask());
          break;
      }
    } else {
      _model.reset();
    }
  }

  void try_trade(Side side, double price) {
    if (FLAGS_enable_trading == false) {
      LOG(WARNING)("Trading *NOT* enabled");
      return;
    }
    // if buy:
    //   if sell order outstanding
    //     cancel old order
    //   if position not long
    //     send buy order
    //
    if (_working_order_id) {
      LOG(INFO)("*** ANOTHER ORDER IS WORKING ***");
      if (side != _working_side) {
        LOG(INFO)("*** CANCEL WORKING ORDER ***");
        _dispatcher.send(
            CancelOrder {
              .account = FLAGS_account,
              .order_id = _working_order_id
            },
            uint8_t{0});
      }
      return;
    }
    if (_instrument.can_trade(side) == false) {
      LOG(INFO)("*** CAN'T INCREASE POSITION ***");
      return;
    }
    auto order_id = ++_max_order_id;
    _dispatcher.send(
        CreateOrder {
          .account = FLAGS_account,
          .order_id = order_id,
          .exchange = FLAGS_exchange,
          .symbol = FLAGS_symbol,
          .side = side,
          .quantity = _instrument.min_trade_vol(),
          .order_type = OrderType::LIMIT,
          .price = price,
          .time_in_force = TimeInForce::GTC,
          .position_effect = PositionEffect::UNDEFINED,
          .order_template = "",
        },
        uint8_t{0});
    _working_order_id = order_id;
    _working_side = side;
    // possible extension: monitor for request timeout
  }

 private:
  client::Dispatcher& _dispatcher;
  Instrument _instrument;
  uint32_t _max_order_id = 0;
  Model _model;
  std::chrono::nanoseconds _next_sample = {};
  uint32_t _working_order_id = 0;
  Side _working_side = Side::UNDEFINED;
};

// application

class Controller final : public Application {
 public:
  using Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected exactly one argument");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    if (FLAGS_simulation) {
      // note! interface has not been finalised
      auto generator =
        client::detail::SimulationFactory::create_generator(
            connections);
        auto matcher = client::detail::SimulationFactory::create_matcher(
            "simple",
            FLAGS_exchange,
            std::chrono::milliseconds{1},
            std::chrono::milliseconds{1});
        auto collector = client::detail::SimulationFactory::create_collector(
            std::chrono::seconds{1});
      client::Simulator(config, *generator, *matcher, *collector)
        .dispatch<Strategy>();
    } else {
      client::Trader(config, connections).dispatch<Strategy>();
    }
    return EXIT_SUCCESS;
  }
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq

namespace {
constexpr std::string_view DESCRIPTION = "Example 3 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_3::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
