/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include "roq/samples/example-2/strategy.h"

#include "roq/samples/example-2/flags.h"

namespace roq {
namespace samples {
namespace example_2 {

Strategy::Strategy(client::Dispatcher &dispatcher)
    : dispatcher_(dispatcher), futures_(Flags::futures_exchange(), Flags::futures_symbol()),
      cash_(Flags::cash_exchange(), Flags::cash_symbol()) {
}

void Strategy::operator()(const Event<Connected> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<Disconnected> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadBegin> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<DownloadEnd> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<GatewayStatus> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<ReferenceData> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<MarketStatus> &event) {
  dispatch(event);
}

void Strategy::operator()(const Event<MarketByPriceUpdate> &event) {
  dispatch(event);
  if (futures_.is_ready() && cash_.is_ready()) {
    // TODO(thraneh): compute basis
  }
}

// helper - dispatch event to the relevant instrument
template <typename T>
void Strategy::dispatch(const T &event) {
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
