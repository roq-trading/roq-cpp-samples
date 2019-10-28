/* Copyright (c) 2017-2019, Hans Erik Thrane */

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
    "BTC-27DEC19",
    "symbol");

DEFINE_string(account,
    "A1",
    "account");

DEFINE_uint32(sample_freq_secs,
    uint32_t{1},
    "sample frequency (seconds)");

DEFINE_double(ema_alpha,
    double{0.33},
    "alpha used to compute exponential moving average (0 < alpha <= 1)");
// exponential moving average (ema) reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

DEFINE_bool(enable_trading,
    false,
    "trading must explicitly be enabled!");

DEFINE_int32(tick_offset,
    -1,
    "offset against best bid (in ticks)");

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
          .exchange = FLAGS_exchange,
          .regex = FLAGS_symbol,
        });
  }

 private:
  Config(const Config&) = delete;
  void operator=(const Config&) = delete;
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
        _depth_builder(client::DepthBuilder::create(_depth)) {
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

  void operator()(const ConnectionStatus& connection_status) {
    if (update(_connection_status, connection_status)) {
      LOG(INFO)(
          "[{}:{}] connection_status={}",
          _exchange,
          _symbol,
          _connection_status);
      check_ready();
    }
    switch (connection_status) {
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
        "[{}:{}] download={}",
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
        "[{}:{}] download={}",
          _exchange,
          _symbol,
        _download);
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
          "[{}:{}] tick_size={}",
          _exchange,
          _symbol,
          _tick_size);
    }
    if (update(_min_trade_vol, reference_data.min_trade_vol)) {
      LOG(INFO)(
          "[{}:{}] min_trade_vol={}",
          _exchange,
          _symbol,
          _min_trade_vol);
    }
    if (update(_multiplier, reference_data.multiplier)) {
      LOG(INFO)(
          "[{}:{}] multiplier={}",
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
          "[{}:{}] trading_status={}",
          _exchange,
          _symbol,
          _trading_status);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const MarketDataStatus& market_data_status) {
    // update our cache
    if (update(_market_data_status, market_data_status.status)) {
      LOG(INFO)(
          "[{}:{}] market_data_status={}",
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
          "[{}:{}] market_data_status={}",
          _exchange,
          _symbol,
          _market_data_status);
    }
    // update the ready flag
    check_ready();
  }

  void operator()(const MarketByPrice& market_by_price) {
    assert(_exchange.compare(market_by_price.exchange) == 0);
    assert(_symbol.compare(market_by_price.symbol) == 0);
    LOG_IF(INFO, _download)("MarketByPrice={}", market_by_price);
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
        "[{}:{}] depth=[{}]",
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    validate(_depth);
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
        "[{}:{}] ready={}",
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
    _ready = false;
  }

  void validate(const Depth& depth) {
    if (std::fabs(depth[0].bid_quantity) < TOLERANCE ||
        std::fabs(depth[0].ask_quantity) < TOLERANCE)
      return;
    auto spread = depth[0].ask_price - depth[0].bid_price;
    LOG_IF(FATAL, spread < TOLERANCE)(
        "[{}:{}] Probably something wrong: "
        "choice or inversion detected. depth=[{}]",
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
  bool _ready = false;
};

// helper

class EMA final {
 public:
  EMA(double alpha)
      : _alpha(alpha) {
  }

  EMA(EMA&&) = default;

  operator double() const {
    return _value;
  }

  void reset() {
    _value = NaN;
  }

  double update(double value) {
    if (std::isnan(_value))
      _value = value;
    else
      _value = _alpha * value + (1.0 - _alpha) * _value;
    return _value;
  }

 private:
  EMA(const EMA&) = delete;
  void operator=(const EMA&) = delete;

 private:
  const double _alpha;
  double _value = NaN;
};

// model implementation

class Model final {
 public:
  Model()
      : _bid_ema(FLAGS_ema_alpha),
        _ask_ema(FLAGS_ema_alpha) {
  }

  Model(Model&&) = default;

  void reset() {
    _bid_ema.reset();
    _ask_ema.reset();
    _selling = false;
    _buying = false;
  }

  Side update(const Depth& depth) {
    if (validate(depth) == false)
      return Side::UNDEFINED;
    auto bid_fast = weighted_bid(depth);
    auto bid_slow = _bid_ema.update(bid_fast);
    auto ask_fast = weighted_ask(depth);
    auto ask_slow = _ask_ema.update(ask_fast);

    if (_selling) {
      if (ask_fast > ask_slow) {
        LOG(INFO)("DEBUG: BUY @ {}", depth[0].ask_price);
        _selling = false;
      }
    } else {
      if (ask_fast < bid_slow && ask_fast < ask_slow) {
        LOG(INFO)("DEBUG: SELLING");
        _selling = true;
        _buying = false;
      }
    }

    if (_buying) {
      if (bid_fast > bid_slow) {
        LOG(INFO)("DEBUG: SELL @ {}", depth[0].bid_price);
        _buying = false;
      }
    } else {
      if (bid_fast > ask_slow && bid_fast > bid_slow) {
        LOG(INFO)("DEBUG: BUYING");
        _buying = true;
        _selling = false;
      }
    }

    // can't be both
    assert(2 != ((_selling ? 1 : 0) + (_buying ? 1 : 0)));

    LOG(INFO)(
        "model={{bid={} ask={} bid_fast={} ask_fast={} bid_slow={} ask_slow={} selling={} buying={}}}",
        depth[0].bid_price,
        depth[0].ask_price,
        bid_fast,
        ask_fast,
        bid_slow,
        ask_slow,
        _selling,
        _buying);

    return Side::UNDEFINED;
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
  Model(const Model&) = delete;
  void operator=(const Model&) = delete;

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
    if (_countdown && 0 == --_countdown) {
      switch (++_stage) {
        case 1:
          // 3x quantity and improves by 1 tick
          _dispatcher.send(
              ModifyOrder {
                .account = FLAGS_account,
                .order_id = _max_order_id,
                .quantity = 3.0 * _instrument.min_trade_vol(),
                .price = _price + _instrument.tick_size(),
              },
              uint8_t{0});
          _countdown = 10;
          break;
        case 2:
          _dispatcher.send(
              CancelOrder {
                .account = FLAGS_account,
                .order_id = _max_order_id,
              },
              uint8_t{0});
          break;
      }
    }
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
          "max_order_id={}",
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
  void operator()(const CreateOrderAckEvent& event) override {
    LOG(INFO)("CreateOrderAck={}", event_value(event));
  }
  void operator()(const ModifyOrderAckEvent& event) override {
    LOG(INFO)("ModifyOrderAck={}", event_value(event));
  }
  void operator()(const CancelOrderAckEvent& event) override {
    LOG(INFO)("CancelOrderAck={}", event_value(event));
  }
  void operator()(const OrderUpdateEvent& event) override {
    LOG(INFO)("OrderUpdate={}", event_value(event));
    _countdown = 10;
  }
  void operator()(const TradeUpdateEvent& event) override {
    LOG(INFO)("TradeUpdate={}", event_value(event));
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

      // DEBUG
      if (!_latch) {
        _latch = true;
        if (FLAGS_enable_trading) {
          const auto& depth = static_cast<const Depth&>(_instrument);
          _price = depth[0].bid_price +
            FLAGS_tick_offset * _instrument.tick_size();
          _dispatcher.send(
              CreateOrder {
                .account = FLAGS_account,
                .order_id = ++_max_order_id,
                .exchange = FLAGS_exchange,
                .symbol = FLAGS_symbol,
                .side = Side::BUY,
                .quantity = _instrument.min_trade_vol(),
                .order_type = OrderType::LIMIT,
                .price = _price,
                .time_in_force = TimeInForce::GTC,
                .position_effect = PositionEffect::UNDEFINED,
                .order_template = std::string(),
              },
              uint8_t{0});
        } else {
          LOG(WARNING)("Trading not enabled");
        }
      }
    } else {
      _model.reset();
    }
  }

 private:
  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  client::Dispatcher& _dispatcher;
  Instrument _instrument;
  uint32_t _max_order_id = 0;
  Model _model;
  std::chrono::nanoseconds _next_sample = {};

  bool _latch = false;
  int _stage = 0;
  int _countdown = 0;
  double _price = 0.0;
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
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_3
}  // namespace samples
}  // namespace roq

namespace {
constexpr const char *DESCRIPTION = "Example 2 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_3::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
