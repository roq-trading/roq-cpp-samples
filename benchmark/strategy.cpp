/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <benchmark/benchmark.h>

#include "roq/logging.h"

#include "roq/samples/common/strategy.h"

namespace {
static auto create_message_info() {
  return roq::MessageInfo {
    .source = 0,
    .source_name = "CFFEX",
    .source_seqno = 1,
    .receive_time_utc = std::chrono::nanoseconds(),
    .receive_time = std::chrono::nanoseconds(),
    .source_send_time = std::chrono::nanoseconds(),
    .source_receive_time = std::chrono::nanoseconds(),
    .origin_create_time = std::chrono::nanoseconds(),
    .is_last = true,
    .opaque = 0
  };
}
static auto create_session_statistics(const char *symbol) {
  return roq::SessionStatistics {
    .exchange = "CFFEX",
    .symbol = symbol,
    .pre_open_interest = 123.45,
    .pre_settlement_price = 123.45,
    .pre_close_price = 123.45,
    .highest_traded_price = 123.45,
    .lowest_traded_price = 123.45,
    .upper_limit_price = 123.45,
    .lower_limit_price = 123.45,
    .exchange_time_utc = std::chrono::milliseconds(1234567890),
  };
}
static auto create_daily_statistics(const char *symbol) {
  return roq::DailyStatistics {
    .exchange = "CFFEX",
    .symbol = symbol,
    .open_price = 123.45,
    .settlement_price = 123.45,
    .close_price = 123.45,
    .open_interest = 123.45,
    .exchange_time_utc = std::chrono::milliseconds(1234567890),
  };
}
static auto create_market_by_price(const char *symbol) {
  static roq::MBPUpdate bid[5], ask[5];
  roq::MarketByPrice result {
    .exchange = "CFFEX",
    .symbol = symbol,
    .bid_length = std::size(bid),
    .bid = bid,
    .ask_length = std::size(ask),
    .ask = ask,
    .snapshot = true,
    .exchange_time_utc = {}
  };
  for (size_t i = 0; i < std::size(bid); ++i) {
    bid[i].price = static_cast<double>(i * 4);
    bid[i].quantity = static_cast<double>(i * 4 + 1);
  }
  for (size_t i = 0; i < std::size(ask); ++i) {
    ask[i].price = static_cast<double>(i * 4 + 2);
    ask[i].quantity = static_cast<double>(i * 4 + 3);
  }
  return result;
}
static auto create_trade_summary(const char *symbol) {
  static roq::Trade trade[5];
  roq::TradeSummary result {
    .exchange = "CFFEX",
    .symbol = symbol,
    .trade_length = std::size(trade),
    .trade = trade,
    .exchange_time_utc = std::chrono::milliseconds(1234567890),
  };
  for (size_t i = 0; i < std::size(trade); ++i) {
    trade[i].price = static_cast<double>(i * 4 + 2);
    trade[i].quantity = static_cast<double>(i * 4 + 3);
    trade[i].side = static_cast<roq::Side>(
        i % static_cast<size_t>(roq::Side::MAX));
  }
  return result;
}
class Dispatcher final : public roq::client::Dispatcher {
 protected:
  void send(const roq::CreateOrder&, uint8_t) override {
  }
  void send(const roq::ModifyOrder&, uint8_t) override {
    throw std::runtime_error("Unexpected");
  }
  void send(const roq::CancelOrder& , uint8_t) override {
    throw std::runtime_error("Unexpected");
  }
  void enqueue(const roq::CustomMessage&) override {
    throw std::runtime_error("Unexpected");
  }
};
static auto create_dispatcher() {
  return Dispatcher();
}
class TestStrategy final : public roq::samples::common::Strategy {
 public:
  using roq::samples::common::Strategy::Strategy;

  size_t get_count() const {
    return _count;
  }

  template <typename E>
  bool dispatch(const E& event) {
    (*this)(event);
    return true;
  }

  bool trade() {
    auto& instrument = at(0);
    instrument.buy_ioc(1.0, 0.1);
    instrument.sell_ioc(1.0, 9999.9);
    return true;
  }

 protected:
  void update(const roq::samples::common::MarketData&) override {
    ++_count;
  }

 private:
  size_t _count = 0;
};
static auto create_strategy(
    roq::client::Dispatcher& dispatcher,
    const roq::samples::common::Config& config) {
  return TestStrategy(dispatcher, config);
}
}  // namespace

void BM_Strategy_Update(benchmark::State& state) {
  // strategy
  auto dispatcher = create_dispatcher();
  roq::samples::common::Config config {
    .instruments = { {
      .exchange = "CFFEX",
      .symbol = "IC1906",
      .net_limit = 1,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = { {
        "A1", {
          .long_limit = 10,
          .short_limit = 10,
          .long_start_of_day = 0.0,
          .short_start_of_day = 0.0,
        }
      } },
    }, {
      .exchange = "CFFEX",
      .symbol = "IF1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {}
    }, {
      .exchange = "CFFEX",
      .symbol = "IH1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {},
    } }
  };
  auto strategy = create_strategy(dispatcher, config);
  // message info
  auto message_info = create_message_info();
  // batch begin event
  auto batch_begin_event = roq::BatchBeginEvent {
    .message_info = message_info
  };
  // batch end event
  auto batch_end_event = roq::BatchEndEvent {
    .message_info = message_info
  };
  // session statistics
  auto session_statistics = create_session_statistics("IC1906");
  auto session_statistics_event = roq::SessionStatisticsEvent {
    .message_info = message_info,
    .session_statistics = session_statistics
  };
  // daily statistics
  auto daily_statistics = create_daily_statistics("IC1906");
  auto daily_statistics_event = roq::DailyStatisticsEvent {
    .message_info = message_info,
    .daily_statistics = daily_statistics
  };
  // market by price
  auto market_by_price = create_market_by_price("IC1906");
  auto market_by_price_event = roq::MarketByPriceEvent {
    .message_info = message_info,
    .market_by_price = market_by_price
  };
  // trade summary
  auto trade_summary = create_trade_summary("IC1906");
  auto trade_summary_event = roq::TradeSummaryEvent {
    .message_info = message_info,
    .trade_summary = trade_summary
  };
  // benchmark
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        strategy.dispatch(batch_begin_event) &&
        strategy.dispatch(session_statistics_event) &&
        strategy.dispatch(daily_statistics_event) &&
        strategy.dispatch(market_by_price_event) &&
        strategy.dispatch(trade_summary_event) &&
        strategy.dispatch(batch_end_event));
  }
  // validate
  if (strategy.get_count() == 0)
    throw std::runtime_error("Unexpected");
}

BENCHMARK(BM_Strategy_Update);

void BM_Strategy_Index(benchmark::State& state) {
  // strategy
  auto dispatcher = create_dispatcher();
  roq::samples::common::Config config {
    .instruments = { {
      .exchange = "CFFEX",
      .symbol = "IC1906",
      .net_limit = 10,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = { {
        "A1", {
          .long_limit = 10,
          .short_limit = 10,
          .long_start_of_day = 0.0,
          .short_start_of_day = 0.0,
        }
      } },
    }, {
      .exchange = "CFFEX",
      .symbol = "IF1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {},
    }, {
      .exchange = "CFFEX",
      .symbol = "IH1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {},
    } }
  };
  auto strategy = create_strategy(dispatcher, config);
  // benchmark
  for (auto _ : state) {
    benchmark::DoNotOptimize(strategy.at(0).can_trade());
  }
}

BENCHMARK(BM_Strategy_Index);

void BM_Strategy_CreateOrder(benchmark::State& state) {
  // strategy
  auto dispatcher = create_dispatcher();
  roq::samples::common::Config config {
    .instruments = { {
      .exchange = "CFFEX",
      .symbol = "IC1906",
      .net_limit = 10,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = { {
        "A1", {
          .long_limit = 10,
          .short_limit = 10,
          .long_start_of_day = 0.0,
          .short_start_of_day = 0.0,
        }
      } },
    }, {
      .exchange = "CFFEX",
      .symbol = "IF1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {},
    }, {
      .exchange = "CFFEX",
      .symbol = "IH1906",
      .net_limit = 0,
      .tick_size = 0.01,
      .multiplier = 100.0,
      .accounts = {},
    } }
  };
  auto strategy = create_strategy(dispatcher, config);
  // message info
  auto message_info = create_message_info();
  // download begin event
  auto download_begin = roq::DownloadBegin {
    .account = "A1"
  };
  auto download_begin_event = roq::DownloadBeginEvent {
    .message_info = message_info,
    .download_begin = download_begin
  };
  // download end event
  auto download_end = roq::DownloadEnd {
    .account = "A1",
    .max_order_id = 0
  };
  auto download_end_event = roq::DownloadEndEvent {
    .message_info = message_info,
    .download_end = download_end
  };
  // order manager status
  auto order_manager_status = roq::OrderManagerStatus {
    .account = "A1",
    .status = roq::GatewayStatus::READY
  };
  auto order_manager_status_event = roq::OrderManagerStatusEvent {
    .message_info = message_info,
    .order_manager_status = order_manager_status
  };
  // initialize
  roq::Logger::initialize();
  strategy.dispatch(download_begin_event);
  strategy.dispatch(order_manager_status_event);
  strategy.dispatch(download_end_event);
  // benchmark
  for (auto _ : state) {
    benchmark::DoNotOptimize(strategy.trade());
  }
  // destruct
  roq::Logger::shutdown();
}

BENCHMARK(BM_Strategy_CreateOrder);
