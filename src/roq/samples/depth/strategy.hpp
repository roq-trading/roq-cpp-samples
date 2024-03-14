/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"
#include "roq/client.hpp"

#include "roq/cache/market_by_price.hpp"

#include "roq/samples/depth/settings.hpp"

namespace roq {
namespace samples {
namespace depth {

struct Strategy final : public client::Handler {
  explicit Strategy(client::Dispatcher &, Settings const &);

  Strategy(Strategy &&) = default;
  Strategy(Strategy const &) = delete;

 protected:
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<GatewaySettings> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;

 private:
  Settings const &settings_;
  std::unique_ptr<cache::MarketByPrice> mbp_full_;
  std::unique_ptr<cache::MarketByPrice> mbp_depth_;
  std::vector<MBPUpdate> bids_, asks_;
  size_t issues_ = {}, choice_ = {}, inversion_ = {};
};

}  // namespace depth
}  // namespace samples
}  // namespace roq
