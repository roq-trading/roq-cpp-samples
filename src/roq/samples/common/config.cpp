/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/common/config.h"

#include "roq/stream.h"

namespace roq {
namespace samples {
namespace common {

void Config::dispatch(Handler& handler) const {
  for (auto& instrument : instruments) {
    for (auto& [name, account] : instrument.accounts) {
      (void) account;  // SO 50176315 -- suppress "unused" warning
      handler.on(
          roq::client::Account {
            .regex = name.c_str(),
          });
    }
    handler.on(
        roq::client::Symbol {
          .exchange = instrument.exchange.c_str(),
          .regex = instrument.symbol.c_str(),
        });
  }
}

std::ostream& operator<<(
    std::ostream& stream,
    const Config::Instrument::Account& account) {
  return stream << "{"
      "long_limit=" << account.long_limit << ", "
      "short_limit=" << account.short_limit << ", "
      "long_start_of_day=" << account.long_start_of_day << ", "
      "short_start_of_day=" << account.short_start_of_day <<
      "}";
}

std::ostream& operator<<(
    std::ostream& stream,
    const Config::Instrument& instrument) {
  return stream << "{"
    "exchange=\"" << instrument.exchange << "\", "
    "symbol=\"" << instrument.symbol << "\", "
    "net_limit=" << instrument.net_limit << ", "
    "tick_size=" << instrument.tick_size << ", "
    "multiplier=" << instrument.multiplier << ", "
    "accounts=" << roq::join(instrument.accounts) <<
    "}";
}

}  // namespace common
}  // namespace samples
}  // namespace roq
