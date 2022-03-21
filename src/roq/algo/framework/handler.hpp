/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace algo {
namespace framework {

class Handler {
 public:
  virtual ~Handler() = default;

  virtual void operator()(const Event<Timer> &) = 0;

  virtual void operator()(const Event<Connected> &) = 0;
  virtual void operator()(const Event<Disconnected> &) = 0;
  virtual void operator()(const Event<DownloadBegin> &) = 0;
  virtual void operator()(const Event<DownloadEnd> &) = 0;
  virtual void operator()(const Event<GatewaySettings> &) = 0;
  virtual void operator()(const Event<GatewayStatus> &) = 0;

  virtual void operator()(const Event<ReferenceData> &) = 0;
  virtual void operator()(const Event<MarketStatus> &) = 0;

  virtual void operator()(const Event<TopOfBook> &) = 0;
  virtual void operator()(const Event<MarketByPriceUpdate> &) = 0;

  virtual void operator()(const Event<OrderAck> &) = 0;
  virtual void operator()(const Event<OrderUpdate> &) = 0;

  virtual void operator()(const Event<PositionUpdate> &) = 0;
};

}  // namespace framework
}  // namespace algo
}  // namespace roq
