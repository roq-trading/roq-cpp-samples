/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <cassert>

#include <array>

#include "roq/logging.h"
#include "roq/service.h"

#include "roq/client.h"

#include "roq/samples/example-2/flags.h"

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
inline bool update(T &lhs, const T &rhs) {
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

  Config(const Config &) = delete;
  Config(Config &&) = default;

 protected:
  void dispatch(Handler &handler) const override {
    // callback for each subscription pattern
    handler(client::Symbol{
        .regex = Flags::futures_symbol(),
        .exchange = Flags::futures_exchange(),
    });
    handler(client::Symbol{
        .regex = Flags::cash_symbol(),
        .exchange = Flags::cash_exchange(),
    });
  }
};

// helper class caching instrument specific information

class Instrument final {
 public:
  Instrument(const std::string_view &exchange, const std::string_view &symbol)
      : exchange_(exchange), symbol_(symbol),
        depth_builder_(client::DepthBuilderFactory::create(symbol, depth_)) {}

  bool isready_() const { return ready_; }

  void operator()(const Connection &connection) {
    if (update(connection_status_, connection.status)) {
      LOG(INFO)
      (R"([{}:{}] connection_status={})",
       exchange_,
       symbol_,
       connection_status_);
      checkready_();
    }
    switch (connection_status_) {
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

  void operator()(const DownloadBegin &download_begin) {
    if (download_begin.account.empty() == false)
      return;
    assert(download_ == false);
    download_ = true;
    LOG(INFO)(R"([{}:{}] download={})", exchange_, symbol_, download_);
  }

  void operator()(const DownloadEnd &download_end) {
    if (download_end.account.empty() == false)
      return;
    assert(download_ == true);
    download_ = false;
    LOG(INFO)(R"([{}:{}] download={})", exchange_, symbol_, download_);
    // update the ready flag
    checkready_();
  }

  void operator()(const MarketDataStatus &market_data_status) {
    // update our cache
    if (update(market_data_status_, market_data_status.status)) {
      LOG(INFO)
      (R"([{}:{}] market_data_status={})",
       exchange_,
       symbol_,
       market_data_status_);
    }
    // update the ready flag
    checkready_();
  }

  void operator()(const ReferenceData &reference_data) {
    assert(exchange_.compare(reference_data.exchange) == 0);
    assert(symbol_.compare(reference_data.symbol) == 0);
    // update the depth builder
    depth_builder_->update(reference_data);
    // update our cache
    if (update(tick_size_, reference_data.tick_size)) {
      LOG(INFO)(R"([{}:{}] tick_size={})", exchange_, symbol_, tick_size_);
    }
    if (update(min_trade_vol_, reference_data.min_trade_vol)) {
      LOG(INFO)
      (R"([{}:{}] min_trade_vol={})", exchange_, symbol_, min_trade_vol_);
    }
    if (update(multiplier_, reference_data.multiplier)) {
      LOG(INFO)(R"([{}:{}] multiplier={})", exchange_, symbol_, multiplier_);
    }
    // update the ready flag
    checkready_();
  }

  void operator()(const MarketStatus &market_status) {
    assert(exchange_.compare(market_status.exchange) == 0);
    assert(symbol_.compare(market_status.symbol) == 0);
    // update our cache
    if (update(trading_status_, market_status.trading_status)) {
      LOG(INFO)
      (R"([{}:{}] trading_status={})", exchange_, symbol_, trading_status_);
    }
    // update the ready flag
    checkready_();
  }

  void operator()(const MarketByPriceUpdate &market_by_price_update) {
    assert(exchange_.compare(market_by_price_update.exchange) == 0);
    assert(symbol_.compare(market_by_price_update.symbol) == 0);
    LOG_IF(INFO, download_)
    (R"(MarketByPriceUpdate={})", market_by_price_update);
    // update depth
    // note!
    //   market by price only gives you *changes*.
    //   you will most likely want to use the the price to look up
    //   the relative position in an order book and then modify the
    //   liquidity.
    //   the depth builder helps you maintain a correct view of
    //   the order book.
    auto depth = depth_builder_->update(market_by_price_update);
    VLOG(1)
    (R"([{}:{}] depth=[{}])", exchange_, symbol_, fmt::join(depth_, ", "));
    if (depth > 0 && isready_())
      update_model();
  }

  void operator()(const MarketByOrderUpdate &market_by_order_update) {
    assert(exchange_.compare(market_by_order_update.exchange) == 0);
    assert(symbol_.compare(market_by_order_update.symbol) == 0);
    LOG_IF(INFO, download_)
    (R"(MarketByOrderUpdate={})", market_by_order_update);
    // update depth
    // note!
    //   market by order only gives you *changes*.
    //   you will most likely want to use the the price and order_id
    //   to look up the relative position in an order book and then
    //   modify the liquidity.
    //   the depth builder helps you maintain a correct view of
    //   the order book.
    auto depth = depth_builder_->update(market_by_order_update);
    VLOG(1)
    (R"([{}:{}] depth=[{}])", exchange_, symbol_, fmt::join(depth_, ", "));
    if (depth > 0 && isready_())
      update_model();
  }

 protected:
  void update_model() {
    // one sided market?
    if (std::fabs(depth_[0].bid_quantity) < TOLERANCE ||
        std::fabs(depth_[0].ask_quantity) < TOLERANCE)
      return;
    // validate depth
    auto spread = depth_[0].ask_price - depth_[0].bid_price;
    LOG_IF(FATAL, spread < TOLERANCE)
    (R"([{}:{}] Probably something wrong: )"
     R"(choice or inversion detected. depth=[{}])",
     exchange_,
     symbol_,
     fmt::join(depth_, ", "));
    // compute (weighted) mid
    double sum_1 = 0.0, sum_2 = 0.0;
    for (auto iter : depth_) {
      sum_1 += iter.bid_price * iter.bid_quantity +
               iter.ask_price * iter.ask_quantity;
      sum_2 += iter.bid_quantity + iter.ask_quantity;
    }
    mid_price_ = sum_1 / sum_2;
    // update (exponential) moving average
    if (std::isnan(avg_price_))
      avg_price_ = mid_price_;
    else
      avg_price_ =
          Flags::alpha() * mid_price_ + (1.0 - Flags::alpha()) * avg_price_;
    // only verbose logging
    VLOG(1)
    (R"([{}:{}] model={{mid_price={}, avg_price={}}})",
     exchange_,
     symbol_,
     mid_price_,
     avg_price_);
  }

  void checkready_() {
    auto before = ready_;
    ready_ = connection_status_ == ConnectionStatus::CONNECTED &&
             download_ == false && tick_size_ > TOLERANCE &&
             min_trade_vol_ > TOLERANCE && multiplier_ > TOLERANCE &&
             trading_status_ == TradingStatus::OPEN &&
             market_data_status_ == GatewayStatus::READY;
    LOG_IF(INFO, ready_ != before)
    (R"([{}:{}] ready={})", exchange_, symbol_, ready_);
  }

  void reset() {
    connection_status_ = ConnectionStatus::DISCONNECTED;
    download_ = false;
    tick_size_ = NaN;
    min_trade_vol_ = NaN;
    trading_status_ = TradingStatus::UNDEFINED;
    market_data_status_ = GatewayStatus::DISCONNECTED;
    depth_builder_->reset();
    mid_price_ = NaN;
    avg_price_ = NaN;
    ready_ = false;
  }

 private:
  const std::string_view exchange_;
  const std::string_view symbol_;
  ConnectionStatus connection_status_ = ConnectionStatus::DISCONNECTED;
  bool download_ = false;
  double tick_size_ = NaN;
  double min_trade_vol_ = NaN;
  double multiplier_ = NaN;
  TradingStatus trading_status_ = TradingStatus::UNDEFINED;
  GatewayStatus market_data_status_ = GatewayStatus::DISCONNECTED;
  std::array<Layer, MAX_DEPTH> depth_;
  std::unique_ptr<client::DepthBuilder> depth_builder_;
  double mid_price_ = NaN;
  double avg_price_ = NaN;
  bool ready_ = false;
};

// strategy implementation

class Strategy final : public client::Handler {
 public:
  explicit Strategy(client::Dispatcher &dispatcher)
      : dispatcher_(dispatcher),
        futures_(Flags::futures_exchange(), Flags::futures_symbol()),
        cash_(Flags::cash_exchange(), Flags::cash_symbol()) {}

  Strategy(const Strategy &) = delete;
  Strategy(Strategy &&) = default;

 protected:
  void operator()(const Event<Connection> &event) override { dispatch(event); }
  void operator()(const Event<DownloadBegin> &event) override {
    dispatch(event);
  }
  void operator()(const Event<DownloadEnd> &event) override { dispatch(event); }
  void operator()(const Event<MarketDataStatus> &event) override {
    dispatch(event);
  }
  void operator()(const Event<ReferenceData> &event) override {
    dispatch(event);
  }
  void operator()(const Event<MarketStatus> &event) override {
    dispatch(event);
  }
  void operator()(const Event<MarketByPriceUpdate> &event) override {
    dispatch(event);
    if (futures_.isready_() && cash_.isready_()) {
      // TODO(thraneh): compute basis
    }
  }

  // helper - dispatch event to the relevant instrument
  template <typename T>
  void dispatch(const T &event) {
    switch (event.message_info.source) {
      case 0:
        futures_(event.value);
        break;
      case 1:
        cash_(event.value);
        break;
      default:
        assert(false);  // should never happen
    }
  }

 private:
  client::Dispatcher &dispatcher_;
  Instrument futures_;
  Instrument cash_;
};

// application

class Controller final : public Service {
 public:
  using Service::Service;

 protected:
  int main_helper(const roq::span<std::string_view> &args) {
    assert(args.empty() == false);
    if (args.size() == 1)
      throw std::runtime_error("Expected arguments");
    if (args.size() != 3)
      throw std::runtime_error(
          "Expected exactly two arguments: "
          "futures exchange then cash exchange");
    Config config;
    // note!
    //   absl::flags will have removed all flags and we're left with arguments
    //   arguments should be a list of unix domain sockets
    auto connections = args.subspan(1);
    client::Trader(config, connections).dispatch<Strategy>();
    return EXIT_SUCCESS;
  }

  int main(int argc, char **argv) override {
    // wrap arguments (prefer to not work with raw pointers)
    std::vector<std::string_view> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
      args.emplace_back(argv[i]);
    return main_helper(args);
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
             argc, argv, DESCRIPTION, ROQ_VERSION)
      .run();
}
