/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/samples/experiment/config.hpp"

namespace roq {
namespace samples {
namespace experiment {

// === HELPERS ===

namespace {
template <typename R>
auto create_accounts(auto &settings) {
  using result_type = std::remove_cvref_t<R>;
  result_type result;
  result.emplace(settings.account);
  return result;
}

template <typename R>
auto create_exchange_symbols(auto &settings) {
  using result_type = std::remove_cvref_t<R>;
  result_type result;
  result[std::string{settings.exchange}].emplace(settings.symbol);
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

Config::Config(Settings const &settings)
    : accounts_{create_accounts<decltype(accounts_)>(settings)}, exchange_symbols_{create_exchange_symbols<decltype(exchange_symbols_)>(settings)} {
}

void Config::dispatch(Handler &handler) const {
  handler(
      client::Settings{
          .order_cancel_policy = OrderCancelPolicy::MANAGED_ORDERS,
          .order_management = {},
      });
  for (auto &account : accounts_) {
    handler(
        client::Account{
            .regex = account,
        });
  }
  for (auto &[exchange, tmp] : exchange_symbols_) {
    for (auto &symbol : tmp) {
      handler(
          client::Symbol{
              .regex = symbol,
              .exchange = exchange,
          });
    }
  }
}

}  // namespace experiment
}  // namespace samples
}  // namespace roq
