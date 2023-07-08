/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/samples/example-2/strategy.hpp"

namespace roq {
namespace samples {
namespace example_2 {

// === IMPLEMENTATION ===

Strategy::Strategy(client::Dispatcher &dispatcher, Settings const &settings)
    : dispatcher_{dispatcher}, futures_{settings.futures_exchange, settings.futures_symbol, settings.alpha},
      cash_{settings.cash_exchange, settings.cash_symbol, settings.alpha} {
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
