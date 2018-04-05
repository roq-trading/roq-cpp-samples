/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "order_management/strategy.h"

namespace examples {
namespace order_management {

void Strategy::update_signal(const roq::MarketByPrice& market_by_price) {
  // pretty simple here ... just keep track of the top-layer
  std::memcpy(&_best, &market_by_price.depth[0], sizeof(decltype(_best)));
}

void Strategy::try_trade() {
  if (is_ready() == false)
    return;
  if (_best.bid_quantity == 0.0 || _best.ask_quantity == 0.0)
    return;
  auto bid_price = _best.bid_price - get_tick_size();
  auto quantity = 1;
  // the intention must be shown: open or close
  if (get_short_position() > 0.0)
    buy_ioc_close(quantity, bid_price);
  else
    buy_ioc_open(quantity, bid_price);
  // note! we expect this order to fail -- buying ioc shouldn't fill on the bid side
}

}  // namespace order_management
}  // namespace examples
