/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <vector>

#include "roq/application.h"
#include "roq/client.h"
#include "roq/logging.h"
#include "roq/format.h"

namespace roq {
namespace samples {
namespace example_1 {

namespace {
constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
template <typename T>
inline bool update(T& lhs, const T& rhs) {
  if (lhs == rhs)  // note! a bit too simplistic for T == double
    return false;
  lhs = rhs;
  return true;
}
}  // namespace

class Config final : public client::Config {
 public:
  Config() {}

 protected:
  void dispatch(Handler& handler) const override {
    // callback for each subscription pattern
    handler.on(
        client::Symbol {
          .exchange = "deribit",
          .regex = "BTC-27DEC19",
        });
    handler.on(
        client::Symbol {
          .exchange = "coinbase-pro",
          .regex = "BTC-USD",
        });
  }

 private:
  Config(Config&&) = default;
  Config(const Config&) = delete;
  void operator=(const Config&) = delete;
};

class Instrument final {
 public:
  Instrument(
      const std::string_view& exchange,
      const std::string_view& symbol)
      : _exchange(exchange),
        _symbol(symbol) {
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
  auto get_best_bid_price() const {
    return _best_bid_price;
  }
  auto get_best_ask() const {
    return _best_ask_price;
  }
  auto get_last_trade_price() const {
    return _last_trade_price;
  }

  void operator()(const roq::ConnectionStatus& connection_status) {
    switch (connection_status) {
      case ConnectionStatus::CONNECTED:
        break;
      case ConnectionStatus::DISCONNECTED:
        _download = false;
        _market_data_status = GatewayStatus::DISCONNECTED;
        _tick_size = NaN;
        _tick_size = NaN;
        _min_trade_vol = NaN;
        _trading_status = TradingStatus::UNDEFINED;
        _best_bid_price = NaN;
        _best_ask_price = NaN;
        _last_trade_price = NaN;
        break;
    }
  }
  void operator()(const roq::DownloadBegin& download_begin) {
    if (download_begin.account.empty() == false)
      return;
    assert(_download == false);
    _download = true;
  }
  void operator()(const roq::DownloadEnd& download_end) {
    if (download_end.account.empty() == false)
      return;
    assert(_download == true);
    _download = false;
  }

  void operator()(const roq::MarketDataStatus& market_data_status) {
    if (update(_market_data_status, market_data_status.status)) {
      LOG(INFO) << fmt::format(
          "[{}:{}] market_data_status={}",
          _exchange,
          _symbol,
          _market_data_status);
    }
  }
  void operator()(const roq::ReferenceData& reference_data) {
    if (update(_tick_size, reference_data.tick_size)) {
      LOG(INFO) << fmt::format(
          "[{}:{}] tick_size={}",
          _exchange,
          _symbol,
          _tick_size);
    }
    if (update(_min_trade_vol, reference_data.min_trade_vol)) {
      LOG(INFO) << fmt::format(
          "[{}:{}] min_trade_vol={}",
          _exchange,
          _symbol,
          _min_trade_vol);
    }
  }
  void operator()(const roq::MarketStatus& market_status) {
    if (update(_trading_status, market_status.trading_status)) {
      LOG(INFO) << fmt::format(
          "[{}:{}] trading_status={}",
          _exchange,
          _symbol,
          _trading_status);
    }
  }
  void operator()(const roq::MarketByPrice& event) {
  }
  void operator()(const roq::TradeSummary& event) {
  }

 private:
  const std::string_view _exchange;
  const std::string_view _symbol;
  bool _download = false;
  GatewayStatus _market_data_status = GatewayStatus::DISCONNECTED;
  double _tick_size = NaN;
  double _min_trade_vol = NaN;
  TradingStatus _trading_status = TradingStatus::UNDEFINED;
  double _best_bid_price = NaN;
  double _best_ask_price = NaN;
  double _last_trade_price = NaN;
};

class Strategy final : public roq::client::Handler {
 public:
  explicit Strategy(roq::client::Dispatcher& dispatcher)
      : _dispatcher(dispatcher),
        _future("deribit", "BTC-27DEC19"),
        _cash("coinbase-pro", "BTC-USD") {
  }

 protected:
  void operator()(const roq::ConnectionStatusEvent& event) override {
    _future(event.connection_status);
  }
  void operator()(const roq::DownloadBeginEvent& event) override {
    _future(event.download_begin);
  }
  void operator()(const roq::DownloadEndEvent& event) override {
    _future(event.download_end);
  }
  void operator()(const roq::MarketDataStatusEvent& event) override {
    _future(event.market_data_status);
  }
  void operator()(const roq::ReferenceDataEvent& event) override {
    _future(event.reference_data);
  }
  void operator()(const roq::MarketStatusEvent& event) override {
    _future(event.market_status);
  }
  void operator()(const roq::MarketByPriceEvent& event) override {
    _future(event.market_by_price);
  }
  void operator()(const roq::TradeSummaryEvent& event) override {
    _future(event.trade_summary);
  }

 private:
  Strategy(Strategy&&) = default;
  Strategy(const Strategy&) = delete;
  void operator=(const Strategy&) = delete;

 private:
  roq::client::Dispatcher& _dispatcher;
  Instrument _future;
  Instrument _cash;
};


class Application final : public roq::Application {
 public:
  using roq::Application::Application;

 protected:
  int main(int argc, char **argv) override {
    if (argc == 1)
      throw std::runtime_error("Expected arguments");
    Config config;
    std::vector<std::string> connections(argv + 1, argv + argc);
    roq::client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }
};

}  // namespace example_1
}  // namespace samples
}  // namespace roq

namespace {
constexpr const char *DESCRIPTION = "Example 1 (Roq Samples)";
}  // namespace

int main(int argc, char **argv) {
  return roq::samples::example_1::Application(
      argc,
      argv,
      DESCRIPTION).run();
}
