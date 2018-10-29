/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "collector/collector.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <iostream>
#include <limits>

namespace examples {
namespace collector {

namespace {
const char *MISSING = "nan";
const char *UPDATE_NAME = "USTP_L2";
const char *UPDATE_TYPE = "1";
const char *DELIMITER = ",";
std::ostream& operator<<(std::ostream& stream, roq::time_point_t time_point) {
  return stream << roq::stream::details::TimePointStr<
      decltype(time_point)>(time_point).c_str();
}
}  // namespace

Collector::Collector(
    roq::Client::Dispatcher& dispatcher,
    const std::string& output_file)
    : _dispatcher(dispatcher) {
  if (!output_file.empty()) {
    _output_file.open(output_file, std::ios::out | std::ios::app);
    if (!_output_file.is_open()) {
      std::stringstream ss;
      ss << "Unable to open file for writing: " << output_file;
      throw std::runtime_error(ss.str());
    }
  }
}

Collector::~Collector() {
  if (_output_file.is_open()) {
    _output_file.close();
  }
}

const roq::Subscriptions& Collector::get_subscriptions() const {
  static roq::Subscriptions result;
  return result;
}

void Collector::on(const roq::BatchEndEvent&) {
  std::ostream& stream = _output_file.is_open() ? _output_file : std::cout;
  for (auto iter : _dirty)
    stream << *iter << std::endl;
  _dirty.clear();
}

void Collector::on(const roq::SessionStatisticsEvent& event) {
  get(event.session_statistics.symbol).update(event);
}

void Collector::on(const roq::DailyStatisticsEvent& event) {
  get(event.daily_statistics.symbol).update(event);
}

void Collector::on(const roq::MarketByPriceEvent& event) {
  get(event.market_by_price.symbol).update(event);
}

void Collector::on(const roq::TradeSummaryEvent& event) {
  get(event.trade_summary.symbol).update(event);
}

Collector::State& Collector::get(const std::string& symbol) {
  auto iter = _cache.find(symbol);
  if (iter == _cache.end())
    iter = _cache.insert({symbol, State(symbol)}).first;
  auto& result = (*iter).second;
  _dirty.insert(&result);
  return result;
}

void Collector::State::update(
    const roq::SessionStatisticsEvent& event) {
  exchange_time = event.session_statistics.exchange_time;
  receive_time = event.message_info.client_receive_time;
  pre_open_interest = event.session_statistics.pre_open_interest;
  pre_settlement_price = event.session_statistics.pre_settlement_price;
  pre_close_price = event.session_statistics.pre_close_price;
  highest_traded_price = event.session_statistics.highest_traded_price;
  lowest_traded_price = event.session_statistics.lowest_traded_price;
  upper_limit_price = event.session_statistics.upper_limit_price;
  lower_limit_price = event.session_statistics.lower_limit_price;
}

void Collector::State::update(
    const roq::DailyStatisticsEvent& event) {
  exchange_time = event.daily_statistics.exchange_time;
  receive_time = event.message_info.client_receive_time;
  open_interest = event.daily_statistics.open_interest;
  open_price = event.daily_statistics.open_price;
}

void Collector::State::update(
    const roq::MarketByPriceEvent& event) {
  exchange_time = event.market_by_price.exchange_time;
  receive_time = event.message_info.source_create_time;
  for (size_t i = 0; i < sizeof(depth) / sizeof(depth[0]); ++i) {
    std::memcpy(
        &depth[i],
        &event.market_by_price.depth[i],
        sizeof(depth[i]));
  }
}

void Collector::State::update(
    const roq::TradeSummaryEvent& event) {
  exchange_time = event.trade_summary.exchange_time;
  receive_time = event.message_info.client_receive_time;
  price = event.trade_summary.price;
  volume = event.trade_summary.volume;
  turnover = event.trade_summary.turnover;
}

std::ostream& operator<<(std::ostream& stream, Collector::State& state) {
  return stream <<
    state.symbol << DELIMITER <<
    state.exchange_time << DELIMITER <<
    state.receive_time << DELIMITER <<
    state.depth[0].ask_price << DELIMITER <<
    state.depth[0].ask_quantity << DELIMITER <<
    state.depth[0].bid_price << DELIMITER <<
    state.depth[0].bid_quantity << DELIMITER <<
    state.depth[1].ask_price << DELIMITER <<
    state.depth[1].ask_quantity << DELIMITER <<
    state.depth[1].bid_price << DELIMITER <<
    state.depth[1].bid_quantity << DELIMITER <<
    state.depth[2].ask_price << DELIMITER <<
    state.depth[2].ask_quantity << DELIMITER <<
    state.depth[2].bid_price << DELIMITER <<
    state.depth[2].bid_quantity << DELIMITER <<
    state.depth[3].ask_price << DELIMITER <<
    state.depth[3].ask_quantity << DELIMITER <<
    state.depth[3].bid_price << DELIMITER <<
    state.depth[3].bid_quantity << DELIMITER <<
    state.depth[4].ask_price << DELIMITER <<
    state.depth[4].ask_quantity << DELIMITER <<
    state.depth[4].bid_price << DELIMITER <<
    state.depth[4].bid_quantity << DELIMITER <<
    state.highest_traded_price << DELIMITER <<
    state.price << DELIMITER <<
    state.lower_limit_price << DELIMITER <<
    state.upper_limit_price << DELIMITER <<
    state.lowest_traded_price << DELIMITER <<
    state.open_interest << DELIMITER <<
    state.open_price << DELIMITER <<
    state.pre_close_price << DELIMITER <<
    state.pre_open_interest << DELIMITER <<
    state.pre_settlement_price << DELIMITER <<
    state.turnover << DELIMITER <<
    state.volume << DELIMITER <<
    UPDATE_NAME << DELIMITER <<
    UPDATE_TYPE;
}

}  // namespace collector
}  // namespace examples
