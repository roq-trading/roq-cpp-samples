/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <cassert>

#include <array>
#include <vector>

#include "roq/application.h"
#include "roq/client.h"
#include "roq/event.h"
#include "roq/logging.h"

// command-line options

DEFINE_string(futures_exchange,
    "deribit",
    "futures exchange");

DEFINE_string(futures_symbol,
    "BTC-27MAR20",
    "futures symbol");

DEFINE_string(cash_exchange,
    "coinbase-pro",
    "cash exchange");

DEFINE_string(cash_symbol,
    "BTC-USD",
    "cash symbol");

DEFINE_double(alpha,
    double{0.2},
    "alpha used to compute exponential moving average");
// reference:
//   https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average

namespace roq {
namespace samples {
namespace example_2 {

// constants

namespace {
constexpr auto NaN = std::numeric_limits<double>::quiet_NaN();
constexpr auto TOLERANCE = double{1.0e-10};
// order book depth
//   we don't actually need 2 layers for this example
constexpr auto MAX_DEPTH = size_t{2};
}  // namespace

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
    // callback for each subscription pattern
    handler(
        client::Symbol {
          .regex = FLAGS_futures_symbol,
          .exchange = FLAGS_futures_exchange,
        });
    handler(
        client::Symbol {
          .regex = FLAGS_cash_symbol,
          .exchange = FLAGS_cash_exchange,
        });
  }
};

// helper class caching instrument specific information

class Instrument final {
 public:
  Instrument(
      const std::string_view& exchange,
      const std::string_view& symbol)
      : _exchange(exchange),
        _symbol(symbol),
        _depth_builder(
            client::DepthBuilder::create(
                symbol,
                _depth)) {
  }

  bool is_ready() const {
    return _ready;
  }

  auto get_tick_size() const {
    return _tick_size;
  }

  auto get_min_trade_vol() const {
    return _min_trade_vol;
  }

  auto get_multiplier() const {
    return _multiplier;
  }

  auto is_market_open() const {
    return _trading_status == TradingStatus::OPEN;
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
    auto depth = _depth_builder->update(market_by_price);
    VLOG(1)(
        FMT_STRING("[{}:{}] depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    if (depth > 0 && is_ready())
      update_model();
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
    auto depth = _depth_builder->update(market_by_order);
    VLOG(1)(
        FMT_STRING("[{}:{}] depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    if (depth > 0 && is_ready())
      update_model();
  }


 protected:
  void update_model() {
    // one sided market?
    if (std::fabs(_depth[0].bid_quantity) < TOLERANCE ||
        std::fabs(_depth[0].ask_quantity) < TOLERANCE)
      return;
    // validate depth
    auto spread = _depth[0].ask_price - _depth[0].bid_price;
    LOG_IF(FATAL, spread < TOLERANCE)(
        FMT_STRING(
          "[{}:{}] Probably something wrong: "
          "choice or inversion detected. depth=[{}]"),
        _exchange,
        _symbol,
        fmt::join(_depth, ", "));
    // compute (weighted) mid
    double sum_1 = 0.0, sum_2 = 0.0;
    for (auto iter : _depth) {
      sum_1 += iter.bid_price * iter.bid_quantity +
        iter.ask_price * iter.ask_quantity;
      sum_2 += iter.bid_quantity + iter.ask_quantity;
    }
    _mid_price = sum_1 / sum_2;
    // update (exponential) moving average
    if (std::isnan(_avg_price))
      _avg_price = _mid_price;
    else
      _avg_price = FLAGS_alpha * _mid_price +
        (1.0 - FLAGS_alpha) * _avg_price;
    // only verbose logging
    VLOG(1)(
        FMT_STRING("[{}:{}] model={{mid_price={}, avg_price={}}}"),
        _exchange,
        _symbol,
        _mid_price,
        _avg_price);
  }

  void check_ready() {
    auto before = _ready;
    _ready =
      _connection_status == ConnectionStatus::CONNECTED &&
      _download == false &&
      _tick_size > TOLERANCE &&
      _min_trade_vol > TOLERANCE &&
      _multiplier > TOLERANCE &&
      _trading_status == TradingStatus::OPEN &&
      _market_data_status == GatewayStatus::READY;
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
    _depth_builder->reset();
    _mid_price = NaN;
    _avg_price = NaN;
    _ready = false;
  }

 private:
  const std::string_view _exchange;
  const std::string_view _symbol;
  ConnectionStatus _connection_status = ConnectionStatus::DISCONNECTED;
  bool _download = false;
  double _tick_size = NaN;
  double _min_trade_vol = NaN;
  double _multiplier = NaN;
  TradingStatus _trading_status = TradingStatus::UNDEFINED;
  GatewayStatus _market_data_status = GatewayStatus::DISCONNECTED;
  std::array<Layer, MAX_DEPTH> _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  double _mid_price = NaN;
  double _avg_price = NaN;
  bool _ready = false;
};

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher),
        _futures(
            FLAGS_futures_exchange,
            FLAGS_futures_symbol),
        _cash(
            FLAGS_cash_exchange,
            FLAGS_cash_symbol) {
  }

  Strategy(const Strategy&) = delete;
  Strategy(Strategy&&) = default;

 protected:
  void operator()(const ConnectionStatusEvent& event) override {
    dispatch(event);
  }
  void operator()(const DownloadBeginEvent& event) override {
    dispatch(event);
  }
  void operator()(const DownloadEndEvent& event) override {
    dispatch(event);
  }
  void operator()(const MarketDataStatusEvent& event) override {
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
    if (_futures.is_ready() && _cash.is_ready()) {
      // TODO(thraneh): compute basis
    }
  }

  // helper - dispatch event to the relevant instrument
  template <typename T>
  void dispatch(const T& event) {
    switch (event.message_info.source) {
      case 0:
        _futures(event_value(event));
        break;
      case 1:
        _cash(event_value(event));
        break;
      default:
        assert(false);  // should never happen
    }
  }

 private:
  client::Dispatcher& _dispatcher;
  Instrument _futures;
  Instrument _cash;
};

// application

class Controller final : public Application {
 public:
  using Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 2)
      throw std::runtime_error(
          "Expected exactly two arguments: "
          "futures exchange then cash exchange");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_2
}  // namespace samples
}  // namespace roq

namespace {
constexpr std::string_view DESCRIPTION = "Example 2 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_2::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
