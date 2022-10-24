/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-2/strategy.hpp"

#include "roq/samples/example-2/flags.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_{dispatcher}, futures_{Flags::futures_exchange(), Flags::futures_symbol()}, cash_{
                                                                                                 Flags::cash_exchange(),
                                                                                                 Flags::cash_symbol()} {
}

void Strategy::operator()(Event<Connected> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<Disconnected> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<DownloadBegin> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<DownloadEnd> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<GatewayStatus> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<ReferenceData> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Strategy::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event);
  if (futures_.is_ready() && cash_.is_ready()) {
    // TODO(thraneh): compute basis
  }
}

// helper - dispatch event to the relevant instrument
template <typename T>
void Strategy::dispatch(Event<T> const &event) {
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

}  // namespace example_2
}  // namespace samples
}  // namespace roq
