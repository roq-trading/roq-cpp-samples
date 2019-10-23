/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <cassert>

#include <array>
#include <vector>

#include "roq/application.h"
#include "roq/client.h"
#include "roq/event.h"
#include "roq/logging.h"


// command-line options

DEFINE_string(future_exchange,
    "deribit",
    "future exchange");

DEFINE_string(future_symbol,
    "BTC-27DEC19",
    "future symbol");

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
  Config(Config&&) = default;

 protected:
  void dispatch(Handler& handler) const override {
    // callback for each subscription pattern
    handler(
        client::Symbol {
          .exchange = FLAGS_future_exchange,
          .regex = FLAGS_future_symbol,
        });
    handler(
        client::Symbol {
          .exchange = FLAGS_cash_exchange,
          .regex = FLAGS_cash_symbol,
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
      const std::string_view& symbol)
      : _exchange(exchange),
        _symbol(symbol),
        _depth_builder(client::DepthBuilder::create(_depth)) {
  }

  auto get_tick_size() const {
    return _tick_size;
  }
  auto get_min_trade_vol() const {
    return _min_trade_vol;
  }
  auto is_market_open() const {
    return _trading_status == TradingStatus::OPEN;;
  }

  void operator()(const ConnectionStatus& connection_status) {
    switch (connection_status) {
      case ConnectionStatus::CONNECTED:
        break;
      case ConnectionStatus::DISCONNECTED:
        // reset all cached state
        _download = false;
        _market_data_status = GatewayStatus::DISCONNECTED;
        _tick_size = NaN;
        _tick_size = NaN;
        _min_trade_vol = NaN;
        _trading_status = TradingStatus::UNDEFINED;
        _depth_builder->reset();
        _mid_price = NaN;
        _avg_price = NaN;
        break;
    }
  }
  void operator()(const DownloadBegin& download_begin) {
    if (download_begin.account.empty() == false)
      return;
    assert(_download == false);
    _download = true;
  }
  void operator()(const DownloadEnd& download_end) {
    if (download_end.account.empty() == false)
      return;
    assert(_download == true);
    _download = false;
  }

  void operator()(const MarketDataStatus& market_data_status) {
    if (update(_market_data_status, market_data_status.status)) {
      LOG(INFO)(
          "[{}:{}] market_data_status={}",
          _exchange,
          _symbol,
          _market_data_status);
    }
  }
  void operator()(const ReferenceData& reference_data) {
    assert(_exchange.compare(reference_data.exchange) == 0);
    assert(_symbol.compare(reference_data.symbol) == 0);
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
  }
  void operator()(const MarketStatus& market_status) {
    assert(_exchange.compare(market_status.exchange) == 0);
    assert(_symbol.compare(market_status.symbol) == 0);
    if (update(_trading_status, market_status.trading_status)) {
      LOG(INFO)(
          "[{}:{}] trading_status={}",
          _exchange,
          _symbol,
          _trading_status);
    }
  }
  void operator()(const MarketByPrice& market_by_price) {
    assert(_exchange.compare(market_by_price.exchange) == 0);
    assert(_symbol.compare(market_by_price.symbol) == 0);
    // update depth
    // note!
    //   market by price only gives you the price and size *changes*.
    //   you will most likely want to use the the price to look up
    //   a relative position in the order book and modify the liquidity.
    //   the depth builder can do that work for you.
    _depth_builder->update(market_by_price);
    VLOG(1)("depth=[{}]", fmt::join(_depth, ", "));
    // now compute mid and average price
    update_model();
  }

 protected:
  void update_model() {
    // compute (weighted) mid
    double sum_1 = 0.0, sum_2 = 0.0;
    for (auto iter : _depth) {
      sum_1 += iter.bid_price * iter.bid_quantity +
        iter.ask_price * iter.ask_quantity;
      sum_2 += iter.bid_quantity + iter.ask_quantity;
    }
    _mid_price = sum_1 / sum_2;
    VLOG(1)("mid_price={}", _mid_price);
    // update (exponential) moving average
    if (std::isnan(_avg_price))
      _avg_price = _mid_price;
    else
      _avg_price = FLAGS_alpha * _mid_price +
        (1.0 - FLAGS_alpha) * _avg_price;
    VLOG(1)("avg_price={}", _avg_price);
  }

 private:
  const std::string_view _exchange;
  const std::string_view _symbol;
  bool _download = false;
  GatewayStatus _market_data_status = GatewayStatus::DISCONNECTED;
  double _tick_size = NaN;
  double _min_trade_vol = NaN;
  TradingStatus _trading_status = TradingStatus::UNDEFINED;
  std::array<Layer, MAX_DEPTH> _depth;
  std::unique_ptr<client::DepthBuilder> _depth_builder;
  double _mid_price = NaN;
  double _avg_price = NaN;
};

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher),
        _future(
            FLAGS_future_exchange,
            FLAGS_future_symbol),
        _cash(
            FLAGS_cash_exchange,
            FLAGS_cash_symbol) {
  }

  Strategy(Strategy&&) = default;

 protected:
  void operator()(const ConnectionStatusEvent& event) override {
    switch (event.source) {
      case 0:
        _future(event.connection_status);
        break;
      case 1:
        _cash(event.connection_status);
        break;
      default:
        assert(false);  // should never happen
    }
  }
  void operator()(const DownloadBeginEvent& event) override {
    update_instrument(event);
  }
  void operator()(const DownloadEndEvent& event) override {
    update_instrument(event);
  }
  void operator()(const MarketDataStatusEvent& event) override {
    update_instrument(event);
  }
  void operator()(const ReferenceDataEvent& event) override {
    update_instrument(event);
  }
  void operator()(const MarketStatusEvent& event) override {
    update_instrument(event);
  }
  void operator()(const MarketByPriceEvent& event) override {
    update_instrument(event);
  }

  // helper - dispatch event to instrument
  template <typename T>
  void update_instrument(const T& event) {
    switch (event.message_info.source) {
      case 0:
        _future(event_value(event));
        break;
      case 1:
        _cash(event_value(event));
        break;
      default:
        assert(false);  // should never happen
    }
  }

 private:
  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  client::Dispatcher& _dispatcher;
  Instrument _future;
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
          "future exchange then cash exchange");
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
constexpr const char *DESCRIPTION = "Example 2 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_2::Controller(
      argc,
      argv,
      DESCRIPTION).run();
}
