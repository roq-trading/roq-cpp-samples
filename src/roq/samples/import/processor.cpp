/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/samples/import/processor.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "roq/utils/compare.hpp"

#include "roq/samples/import/base64.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

namespace roq {
namespace samples {
namespace import {

// === CONSTANTS ===

namespace {
auto const EXCHANGE = "CME"sv;
auto const SYMBOL = "GEZ1"sv;
auto const TICK_SIZE = 0.0025;
auto const MULTIPLIER = 2500.0;
auto const MIN_TRADE_VOL = 1.0;  // 1 lot
}  // namespace

// === HELPERS ===

namespace {
auto create_encoder() {
  auto options = codec::fbs::Encoder::Options{
      .size_prefixed = true,
  };
  return codec::fbs::Encoder::create(options);
}

auto use_base64(auto &encoding) {
  if (utils::case_insensitive_compare(encoding, "binary"sv) == 0)
    return false;
  if (utils::case_insensitive_compare(encoding, "base64"sv) == 0)
    return true;
  throw RuntimeError{R"(Unknown encoding="{}")"sv, encoding};
}
}  // namespace

// === IMPLEMENTATION ===

Processor::Processor(Settings const &settings, std::string_view const &path)
    : encoder_{create_encoder()}, file_{std::string{path}, std::ios::out | std::ios::binary},
      encoding_{use_base64(settings.encoding) ? Encoding::BASE64 : Encoding::BINARY} {
  if (!file_)
    throw RuntimeError{R"(Unable to open file for writing: path="{}")"sv, path};
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
  // first message *must* be GatewaySettings
  process(
      GatewaySettings{
          .supports = {},
          .mbp_max_depth = 3,
          .mbp_tick_size_multiplier = NaN,
          .mbp_min_trade_vol_multiplier = NaN,
          .mbp_allow_remove_non_existing = false,
          .mbp_allow_price_inversion = false,
          .oms_download_has_state = false,
          .oms_download_has_routing_id = false,
          .oms_request_id_type = {},
          .oms_cancel_all_orders = {},
      },
      1ns);  // timestamp should be something useful, like UTC
  // prefer to process ReferenceData before any market data
  process(
      ReferenceData{
          .stream_id = {},       // has little significance for simulation
          .exchange = EXCHANGE,  // required
          .symbol = SYMBOL,      // required
          .description = {},
          .security_type = {},
          .base_currency = {},
          .quote_currency = {},
          .settlement_currency = {},
          .margin_currency = {},
          .commission_currency = {},
          .tick_size = TICK_SIZE,    // strongly recommended
          .multiplier = MULTIPLIER,  // useful
          .min_notional = NaN,
          .min_trade_vol = MIN_TRADE_VOL,  // strongly recommended
          .max_trade_vol = NaN,
          .trade_vol_step_size = NaN,
          .option_type = {},
          .strike_currency = {},
          .strike_price = NaN,
          .underlying = {},
          .time_zone = {},
          .issue_date = {},
          .settlement_date = {},
          .expiry_datetime = {},
          .expiry_datetime_utc = {},
          .exchange_time_utc = {},
          .exchange_sequence = {},
          .sending_time_utc = {},
          .discard = {},
      },
      2ns);
  // prefer to publish market trading status
  process(
      MarketStatus{
          .stream_id = {},
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .trading_status = TradingStatus::OPEN,
          .exchange_time_utc = {},
          .exchange_sequence = {},
          .sending_time_utc = {},
      },
      3ns);
  // initial image
  // ... prefer to sort bids descending
  MBPUpdate bids_image[] = {
      {
          .price = 99.785,
          .quantity = 3.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
      {
          .price = 99.780,
          .quantity = 2.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
      {
          .price = 99.775,
          .quantity = 1.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
  };
  // ... prefer to sort asks asscending
  MBPUpdate asks_image[] = {
      {
          .price = 99.800,
          .quantity = 3.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
      {
          .price = 99.805,
          .quantity = 2.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
      {
          .price = 99.810,
          .quantity = 1.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
  };
  process(
      MarketByPriceUpdate{
          .stream_id = {},
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .bids = bids_image,
          .asks = asks_image,
          .update_type = UpdateType::SNAPSHOT,  // indicates that it's an *image*
          .exchange_time_utc = {},              // probably similar to the timestamp you're using
          .exchange_sequence = {},
          .price_precision = {},
          .quantity_precision = {},
          .max_depth = {},
          .checksum = {},
      },
      4ns);
  // update
  MBPUpdate bids_update[] = {
      // remove best price ...
      {
          .price = 99.785,
          .quantity = 0.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
      // ... then introduce new price
      {
          .price = 99.770,
          .quantity = 2.0,
          .implied_quantity = NaN,
          .number_of_orders = {},
          .update_action = {},
          .price_level = {},
      },
  };
  process(
      MarketByPriceUpdate{
          .stream_id = {},
          .exchange = EXCHANGE,
          .symbol = SYMBOL,
          .bids = bids_update,
          .asks = {},
          .update_type = UpdateType::INCREMENTAL,  // indicates that it's an *update*
          .exchange_time_utc = {},                 // probably similar to the timestamp you're using
          .exchange_sequence = {},
          .price_precision = {},
          .quantity_precision = {},
          .max_depth = {},
          .checksum = {},
      },
      5ns);
}

MessageInfo Processor::create_message_info(std::chrono::nanoseconds timestamp_utc) {
  // note! just re-use the same timestamp for all trace points
  return {
      .source = {},             // not encoded
      .source_name = {},        // not encoded
      .source_session_id = {},  // not encoded
      .source_seqno = ++seqno_,
      .receive_time_utc = timestamp_utc,
      .receive_time = timestamp_utc,
      .source_send_time = timestamp_utc,
      .source_receive_time = timestamp_utc,
      .origin_create_time = timestamp_utc,
      .origin_create_time_utc = timestamp_utc,
      .is_last = true,  // messages can be batched
      .opaque = {},     // unused
  };
}

template <typename T>
void Processor::process(T const &value, std::chrono::nanoseconds timestamp_utc) {
  auto message_info = create_message_info(timestamp_utc);
  Event event{message_info, value};
  auto message = (*encoder_)(event);
  assert(!std::empty(message));
  file_.write(reinterpret_cast<char const *>(std::data(message)), std::size(message));
}

}  // namespace import
}  // namespace samples
}  // namespace roq
