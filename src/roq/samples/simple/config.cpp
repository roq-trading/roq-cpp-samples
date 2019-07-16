/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/samples/simple/config.h"

#include "roq/samples/common/tokenizer.h"

namespace roq {
namespace samples {
namespace simple {

namespace {
static auto tokenize(const auto& value) {
  auto values = roq::samples::common::tokenize(value, ",");
  return std::unordered_set<std::string> {
      values.begin(),
      values.end()
  };
}
static auto create_accounts(const auto& accounts) {
  return tokenize(accounts);
}
static auto create_symbols_by_exchange(
    const auto& exchange,
    const auto& symbols) {
  return std::unordered_map<
    std::string,
    std::unordered_set<std::string> > { {
      exchange,
      tokenize(symbols)
  } };
}
}  // namespace

Config::Config(
    const std::string& accounts,
    const std::string& exchange,
    const std::string& symbols,
    bool create_orders,
    std::chrono::milliseconds order_timeout)
    : _accounts(
        create_accounts(accounts)),
      _symbols_by_exchange(
          create_symbols_by_exchange(exchange, symbols)),
      _create_orders(create_orders),
      _order_timeout(order_timeout) {
}

void Config::dispatch(Handler& handler) const {
  for (auto& account : _accounts) {
    handler.on(
        roq::client::Account {
          .regex = account.c_str(),
        });
  }
  for (auto& [exchange, symbols] : _symbols_by_exchange) {
    for (auto& symbol : symbols) {
      handler.on(
          roq::client::Symbol {
            .exchange = exchange.c_str(),
            .regex = symbol.c_str(),
          });
    }
  }
}

}  // namespace simple
}  // namespace samples
}  // namespace roq
