/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-7/strategy.hpp"

#include "roq/logging.hpp"

#include "roq/samples/example-7/flags.hpp"

using namespace std::literals;

namespace roq {
namespace samples {
namespace example_7 {

/*
 * note!
 * Measurement.name is a string *buffer*
 * This means that every assignment is a copy
 * For smaller strings that's fine, but unnecessary
 * We therefore pre-allocate the array here at construction time
 */
Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher), measurements_{{
                                   {.name = "bp"sv, .value = NaN},
                                   {.name = "ap"sv, .value = NaN},
                               }} {
}

void Strategy::operator()(const Event<TopOfBook> &event) {
  auto &[message_info, top_of_book] = event;
  log::info<1>("[{}:{}] TopOfBook={}"sv, message_info.source, message_info.source_name, top_of_book);
  measurements_[0].value = top_of_book.layer.bid_price;
  measurements_[1].value = top_of_book.layer.ask_price;
  CustomMetrics custom_metrics{
      .label = Flags::label(),
      .account = Flags::account(),
      .exchange = top_of_book.exchange,
      .symbol = top_of_book.symbol,
      .measurements = measurements_,
  };
  dispatcher_.send(custom_metrics, 0);
}

}  // namespace example_7
}  // namespace samples
}  // namespace roq
