/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "utilities/generator.h"

#include <roq/logging.h>
#include <roq/stream.h>

#include <gflags/gflags.h>

#include <limits>

namespace examples {
namespace utilities {

DEFINE_bool(new_time_format, false, "Use new time format?");

const size_t MAX_COLUMNS = 40;
const char *TIME_FORMAT_FILE_OLD = "%Y%m%d %H:%M:%S";
const char *TIME_FORMAT_FILE_NEW = "%Y-%m-%dT%H:%M:%S";

const char *EXCHANGE = "CFFEX";
const uint32_t L1_TOPIC_ID = 100;  // CFFEX L1
const uint32_t L2_TOPIC_ID = 110;  // CFFEX L2

const double MISSING = 0.0;

Generator::Generator(const std::string& path)
    : _time_format(
        FLAGS_new_time_format ? TIME_FORMAT_FILE_NEW
                              : TIME_FORMAT_FILE_OLD),
      _csv_reader(path, MAX_COLUMNS) {
}

Generator::~Generator() {
  LOG(INFO) << "Processed " << _message_id << " message(s)";
}

std::pair<bool, std::chrono::system_clock::time_point> Generator::fetch() {
  if (!_csv_reader.fetch())
    return std::make_pair(false, std::chrono::system_clock::time_point());
  ++_message_id;
  auto receive_time = _csv_reader.get_time_point(2, _time_format);
  LOG_IF(FATAL, receive_time < _receive_time) << "Incorrect sequencing";
  _receive_time = receive_time;
  return std::make_pair(true, _receive_time);
}

void Generator::dispatch(
    roq::simulation::Generator::Dispatcher& dispatcher) {
  auto symbol = _csv_reader.get_string(0);
  auto exchange_time = _csv_reader.get_time_point(1, _time_format);
  auto receive_time = _csv_reader.get_time_point(2, _time_format);
  auto type = _csv_reader.get_integer(_csv_reader.length() - 1);  // last column is indicator for L1/L2
  switch (type) {
    case 0: return;  // L1 (don't process, for now)
    case 1: break;
    default: LOG(FATAL) << "Invalid type=" << type;
  }
  roq::MessageInfo message_info = {
    .source = "simulator",
    .source_create_time = receive_time,
    .client_receive_time = receive_time,
    .routing_latency = std::chrono::microseconds(0),
    .from_cache = false,
    .is_last = false,
  };
  dispatcher.on(roq::BatchBeginEvent { .message_info = message_info});
  roq::MarketByPrice market_by_price = {
    .exchange = EXCHANGE,
    .symbol = symbol.c_str(),
    .depth = {},
    .total_bid_volume = MISSING,
    .total_ask_volume = MISSING,
    .exchange_time = exchange_time,
    .channel = L2_TOPIC_ID,
  };
  for (auto i = 0; i < 5; ++i) {
    auto offset = 3 + (i * 4);
    roq::Layer& layer = market_by_price.depth[i];
    layer.ask_price = _csv_reader.get_number(offset + 0);
    layer.ask_quantity = _csv_reader.get_number(offset + 1);
    layer.bid_price = _csv_reader.get_number(offset + 2);
    layer.bid_quantity = _csv_reader.get_number(offset + 3);
  }
  VLOG(3) << market_by_price;
  dispatcher.on(roq::MarketByPriceEvent {
      .message_info = message_info,
      .market_by_price = market_by_price });
  roq::TradeSummary trade_summary = {
    .exchange = EXCHANGE,
    .symbol = symbol.c_str(),
    .price = _csv_reader.get_number(24),
    .volume = _csv_reader.get_number(34),
    .turnover = _csv_reader.get_number(33),
    .side = roq::Side::Undefined,
    .exchange_time = exchange_time,
    .channel = L2_TOPIC_ID,
  };
  VLOG(3) << trade_summary;
  message_info.is_last = true;
  dispatcher.on(roq::TradeSummaryEvent {
      .message_info = message_info,
      .trade_summary = trade_summary });
  // TODO(thraneh): SessionStatistics
  // TODO(thraneh): DailyStatistics
  dispatcher.on(roq::BatchEndEvent { .message_info = message_info });
}

}  // namespace utilities
}  // namespace examples
