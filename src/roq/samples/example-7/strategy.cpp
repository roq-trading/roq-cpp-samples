/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-7/strategy.hpp"

#include <algorithm>

#include "roq/logging.hpp"

#include "roq/samples/example-7/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_7 {

// === IMPLEMENTATION ===

// note!
// Measurement.name is a string *buffer*
// This means that every assignment is a copy
// For smaller strings that's fine, but unnecessary
// We therefore pre-allocate the array here at construction time

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_{dispatcher}, measurements_{{
                                   {
                                       .name = "bp"sv,
                                       .value = NaN,
                                   },
                                   {
                                       .name = "ap"sv,
                                       .value = NaN,
                                   },
                               }},
      rows_{{
          "0"sv,
          "-1"sv,
          "-2"sv,
      }} {
}

void Strategy::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  log::info<0>("[{}:{}] TopOfBook={}"sv, message_info.source, message_info.source_name, top_of_book);
  //  metrics
  measurements_[0].value = top_of_book.layer.bid_price;
  measurements_[1].value = top_of_book.layer.ask_price;
  auto custom_metrics = CustomMetrics{
      .label = Flags::label(),
      .account = Flags::account(),
      .exchange = top_of_book.exchange,
      .symbol = top_of_book.symbol,
      .measurements = measurements_,
      .update_type = UpdateType::INCREMENTAL,
  };
  log::debug("custom_metrics={}"sv, custom_metrics);
  // matrix
  std::shift_right(std::begin(data_), std::end(data_), 1);
  data_[0] = 0.5 * (top_of_book.layer.bid_price + top_of_book.layer.ask_price);
  auto custom_matrix = CustomMatrix{
      .label = Flags::label(),
      .account = Flags::account(),
      .exchange = top_of_book.exchange,
      .symbol = top_of_book.symbol,
      .rows = rows_,
      .columns = {},
      .data = data_,
      .update_type = UpdateType::INCREMENTAL,
      .version = ++version_,
  };
  log::debug("custom_matrix={}"sv, custom_matrix);
  try {
    dispatcher_.send(custom_metrics, 0);
    dispatcher_.send(custom_matrix, 0);
  } catch (NotConnected &) {
  }
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
