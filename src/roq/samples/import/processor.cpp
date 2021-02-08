/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/samples/import/processor.h"

#include <fmt/format.h>

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "roq/fbs/api.h"
#include "roq/fbs/encode.h"

#include "roq/samples/import/base64.h"
#include "roq/samples/import/flags.h"

#include "roq/logging.h"

namespace roq {
namespace samples {
namespace import {

namespace {
static const auto EXCHANGE = "CME";
static const auto SYMBOL = "GEZ1";
static const double TICK_SIZE = 0.0025;
static const double MULTIPLIER = 2500.0;
static const double MIN_TRADE_VOL = 1.0;  // lots
}  // namespace

namespace {
static bool use_base64() {
  auto encoding = Flags::encoding();
  if (encoding.compare("binary") == 0)
    return false;
  if (encoding.compare("base64") == 0)
    return true;
  throw std::runtime_error(fmt::format(R"(Unable to encoding="{}")", encoding));
}
}  // namespace

Processor::Processor(const std::string_view &path)
    : file_(std::string{path}, std::ios::out | std::ios::binary),
      encoding_(use_base64() ? Encoding::BASE64 : Encoding::BINARY) {
  if (!file_)
    throw std::runtime_error(fmt::format(R"(Unable to open file for writing: path="{}")", path));
}

Processor::~Processor() {
  try {
    // best effort
    if (file_.is_open())
      file_.close();
  } catch (...) {
  }
}
void Processor::dispatch() {
  // first message *must* GatewaySettings
  process(
      GatewaySettings{
          .mbp_max_depth = 3,
          .mbp_allow_price_inversion = false,
      },
      std::chrono::nanoseconds{1});  // timestamp should be something useful, like UTC
  // prefer to process ReferenceData before any market data
  process(
      ReferenceData{
          .exchange = EXCHANGE,  // required
          .symbol = SYMBOL,      // required
          .description = {},
          .security_type = {},
          .currency = {},
          .settlement_currency = {},
          .commission_currency = {},
          .tick_size = TICK_SIZE,          // strongly recommended
          .multiplier = MULTIPLIER,        // useful
          .min_trade_vol = MIN_TRADE_VOL,  // strongly recommended
          .option_type = {},
          .strike_currency = {},
          .strike_price = {},
          .underlying = {},
          .time_zone = {},
          .issue_date = {},
          .settlement_date = {},
          .expiry_datetime = {},
          .expiry_datetime_utc = {},
      },
      std::chrono::nanoseconds{2});
  // prefer to publish market trading status
  process(
      MarketStatus{
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .trading_status = TradingStatus::OPEN,
      },
      std::chrono::nanoseconds{3});
  // initial image
  // ... prefer to sort bids descending
  MBPUpdate bids_image[] = {
      {.price = 99.785, .quantity = 3.0},
      {.price = 99.780, .quantity = 2.0},
      {.price = 99.775, .quantity = 1.0},
  };
  // ... prefer to sort asks asscending
  MBPUpdate asks_image[] = {
      {.price = 99.800, .quantity = 3.0},
      {.price = 99.805, .quantity = 2.0},
      {.price = 99.810, .quantity = 1.0},
  };
  process(
      MarketByPriceUpdate{
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .bids = bids_image,
          .asks = asks_image,
          .snapshot = true,         // indicates that it's an *image*
          .exchange_time_utc = {},  // probably similar to the timestamp you're using
      },
      std::chrono::nanoseconds{4});
  // update
  MBPUpdate bids_update[] = {
      {.price = 99.785, .quantity = 0.0},  // remove best price
      {.price = 99.770, .quantity = 2.0},  // introduce new price
  };
  process(
      MarketByPriceUpdate{
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .bids = bids_update,
          .asks = {},
          .snapshot = false,        // indicates that it's an *update*
          .exchange_time_utc = {},  // probably similar to the timestamp you're using
      },
      std::chrono::nanoseconds{5});
}

MessageInfo Processor::create_message_info(std::chrono::nanoseconds timestamp_utc) {
  // note! just re-use the same timestamp for all trace points
  return MessageInfo{
      .source = 0,              // not encoded
      .source_name = {},        // not encoded
      .source_session_id = {},  // not encoded
      .source_seqno = ++seqno_,
      .receive_time_utc = timestamp_utc,
      .receive_time = timestamp_utc,
      .source_send_time = timestamp_utc,
      .source_receive_time = timestamp_utc,
      .origin_create_time = timestamp_utc,
      .origin_create_time_utc = timestamp_utc,
      .is_last = true,
      .opaque = {},  // unused
  };
}

template <typename T>
void Processor::process(const T &value, std::chrono::nanoseconds timestamp) {
  builder_.Clear();
  auto message_info = create_message_info(timestamp);
  Event<T> event(message_info, value);
  auto root = fbs::encode(builder_, event);
  builder_.FinishSizePrefixed(root);  // note! *must* include size
  auto data = builder_.GetBufferPointer();
  auto length = builder_.GetSize();
  switch (encoding_) {
    case Encoding::BINARY:
      file_.write(reinterpret_cast<char const *>(data), length);
      break;
    case Encoding::BASE64: {
      auto message = Base64::encode(data, length);
      file_.write(message.c_str(), message.length());
      break;
    }
  }
}

}  // namespace import
}  // namespace samples
}  // namespace roq
