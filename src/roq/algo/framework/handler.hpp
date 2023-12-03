/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace framework {

struct Handler {
  virtual ~Handler() = default;

  virtual void operator()(Event<Timer> const &) = 0;

  virtual void operator()(Event<Connected> const &) = 0;
  virtual void operator()(Event<Disconnected> const &) = 0;
  virtual void operator()(Event<DownloadBegin> const &) = 0;
  virtual void operator()(Event<DownloadEnd> const &) = 0;
  virtual void operator()(Event<GatewaySettings> const &) = 0;
  virtual void operator()(Event<GatewayStatus> const &) = 0;

  virtual void operator()(Event<ReferenceData> const &) = 0;
  virtual void operator()(Event<MarketStatus> const &) = 0;

  virtual void operator()(Event<TopOfBook> const &) = 0;
  virtual void operator()(Event<MarketByPriceUpdate> const &) = 0;

  virtual void operator()(Event<OrderAck> const &) = 0;
  virtual void operator()(Event<OrderUpdate> const &) = 0;

  virtual void operator()(Event<PositionUpdate> const &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
