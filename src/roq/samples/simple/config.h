/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "roq/client.h"

namespace roq {
namespace samples {
namespace simple {

class Config final : public client::Config {
 public:
  Config(
      const std::string& accounts,
      const std::string& exchange,
      const std::string& symbols,
      bool create_orders,
      std::chrono::milliseconds order_timeout);

  auto& get_accounts() const {
    return _accounts;
  }
  auto& get_symbols_by_exchange() const {
    return _symbols_by_exchange;
  }
  auto create_orders() const {
    return _create_orders;
  }
  auto get_order_timeout() const {
    return _order_timeout;
  }

 protected:
  void dispatch(Handler& handler) const override;

 private:
  const std::unordered_set<std::string> _accounts;
  const std::unordered_map<
    std::string,
    std::unordered_set<std::string> > _symbols_by_exchange;
  const bool _create_orders;
  const std::chrono::milliseconds _order_timeout;
};

}  // namespace simple
}  // namespace samples
}  // namespace roq
