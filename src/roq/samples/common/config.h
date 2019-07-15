/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "roq/client.h"

namespace roq {
namespace samples {
namespace common {

struct Config : public client::Config {
  struct Instrument final {
    std::string exchange;
    std::string symbol;
    double net_limit;
    double tick_size;  // optional
    double multiplier;  // optional
    struct Account final {
      double long_limit;
      double short_limit;
      double long_start_of_day;
      double short_start_of_day;
    };
    std::map<std::string, Account> accounts;  // optional
  };
  std::vector<Instrument> instruments;

  explicit Config(std::vector<Instrument>&& instruments) {
    (*this).instruments = std::move(instruments);
  }

  void dispatch(Handler&) const override;
};

std::ostream& operator<<(
    std::ostream& stream,
    const Config& config);

}  // namespace common
}  // namespace samples
}  // namespace roq
