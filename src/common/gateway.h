/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/account.h"

namespace examples {
namespace common {

class Instrument;

class Gateway final {
 public:
  Gateway(roq::Strategy::Dispatcher& dispatcher, const std::string& name);
  void on(const roq::MarketDataStatusEvent& event);

 private:
  roq::Strategy::Dispatcher& _dispatcher;
  const std::string _name;
};

}  // namespace common
}  // namespace examples
